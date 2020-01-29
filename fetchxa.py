import click
import os
import re
import requests
import subprocess

from lxml import etree
from pathlib import Path


re_xa = re.compile(r'xa-(?P<version>.*)\.tar\.gz')
baseurls = [
    'https://www.floodgap.com/retrotech/xa/dists/unsupported/',
    'https://www.floodgap.com/retrotech/xa/dists/'
]


@click.command()
@click.option('-t', '--target', default='xa', type=Path)
def main(target):
    if not target.is_dir():
        raise click.ClickException('target is not a directory')

    if not (target / '.git').is_dir():
        raise click.ClickException('target does not appear to be '
                                   'a git directory')

    for baseurl in baseurls:
        res = requests.get(baseurl)

        parser = etree.HTMLParser()
        doc = etree.fromstring(res.text, parser=parser)
        links = doc.xpath('//a[starts-with(@href, "xa-") and '
                          'contains(@href, ".tar.gz")]')

        for link in links:
            href = link.get('href')
            mo = re_xa.match(href)
            if mo:
                version = mo.group('version')
                url = '{}/{}'.format(baseurl, href)

                if not os.path.exists(href):
                    print('fetching xa', version)
                    subprocess.check_call(['curl', '-sf', '-o', href, url])

                print('adding files to git for xa ', version)
                subprocess.check_call('rm -rf *', cwd=target, shell=True)
                subprocess.check_call(['tar', '-C', target,
                                       '--strip-components=1', '-xf', href])

                subprocess.check_call(['git', '-C', target,
                                       'add', '-u'])
                subprocess.check_call(['git', '-C', target,
                                       'add', '.'])
                subprocess.check_call(['git', '-C', target,
                                       'commit', '-m', version])
                subprocess.check_call(['git', '-C', target,
                                       'tag', version])


if __name__ == '__main__':
    main()
