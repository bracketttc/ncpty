<!--
Copyright (c) 2020 Timothy Brackett
Licensed under the MIT license
-->

# ncpty - an NCurses pseudoterminal library

<!-- Badges -->
[![Gitpod ready-to-code][gitpod-badge]][gitpod]
[![CII Best Practices][cii-badge]][cii]
[![Gitlab][pipeline]][home]
[![coveralls.io][coveralls-badge]][coveralls]
[![Coverity][coverity-badge]][coverity]
[![LGTM][lgtm-alerts-badge]][lgtm-alerts]
[![LGTM][lgtm-quality-badge]][lgtm-quality]
[![MIT License][license-badge]][license]
[![readthedocs][readthedocs-badge]][readthedocs]
[![Gitter][gitter-badge]][gitter]

Based on the [libvterm][libvterm] abstract terminal emulator.
ncpty aims to make it easier to embed other console/TUI programs inside of an ncurses interface.

The example project, `banner`, wraps a terminal command or shell in classification banners like one might see in a US government classified GUI or web site.

## Building

ncpty is built using CMake.
Dependencies are ncurses, panel, and vterm.

## Contributing

Help and/or feedback is appreciated.
See the [CONTRIBUTING](./CONTRIBUTING.md) page.

## Bug / Vulnerability Reporting

All issues may be reported using the Gitlab [Issues page][issues].

<!-- Links -->
[cii-badge]: https://bestpractices.coreinfrastructure.org/projects/4967/badge
[cii]: https://bestpractices.coreinfrastructure.org/projects/4967
[coveralls-badge]: https://coveralls.io/repos/gitlab/bracketttc/ncpty/badge.svg
[coveralls]: https://coveralls.io/gitlab/bracketttc/ncpty
[coverity-badge]: https://scan.coverity.com/projects/20891/badge.svg?flat=1
[coverity]: https://scan.coverity.com/projects/bracketttc-ncpty
[gitpod-badge]: https://img.shields.io/badge/Gitpod-ready--to--code-blue?logo=gitpod
[gitpod]: https://gitpod.io/#https://gitlab.com/bracketttc/ncpty
[gitter-badge]: https://badges.gitter.im/bracketttc-ncpty/community.svg
[gitter]: https://gitter.im/bracketttc-ncpty/community?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge
[home]: https://gitlab.com/bracketttc/ncpty
[issues]: https://gitlab.com/bracketttc/ncpty/-/issues
[lgtm-alerts-badge]: https://img.shields.io/lgtm/alerts/g/bracketttc/ncpty.svg?logo=lgtm&logoWidth=18
[lgtm-alerts]: https://lgtm.com/projects/g/bracketttc/ncpty/alerts/
[lgtm-quality-badge]: https://img.shields.io/lgtm/grade/cpp/g/bracketttc/ncpty.svg?logo=lgtm&logoWidth=18
[lgtm-quality]: https://lgtm.com/projects/g/bracketttc/ncpty/context:cpp
[license-badge]: https://img.shields.io/badge/License-MIT-blue.svg
[license]: https://opensource.org/licenses/MIT
[pipeline]: https://gitlab.com/bracketttc/ncpty/badges/devel/pipeline.svg
[readthedocs-badge]: https://readthedocs.org/projects/ncpty/badge/
[readthedocs]: https://ncpty.readthedocs.io

[libvterm]: https://launchpad.net/libvterm
