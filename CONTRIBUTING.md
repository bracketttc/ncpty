<!--
Copyright (c) 2020 Timothy Brackett
Licensed under the MIT license
-->

# Contributing

Feedback and contributions are welcome.

## General information

For specific proposals or change requests, please use the [issues][issues] and [pull requests][pull requests] pages on the [Gitlab site][home].
For general discussion or support, feel free to join the conversation on [Gitter][gitter].

### License

All contributed material must be released under the [MIT license][MIT license].

### Code style

Reformat code using `clang-format` before committing.
Refer to [`clang-format`][clang-format] [specification](.clang-format) for specific details.

### Git commit style

A good reference for commit style is [Chris Beams blog post][chris-beams].
Main points:
 - Separate subject from body with an blank line
 - Limit the subject line to 50 characters
 - Capitalize the subject line
 - Don't end the subject line with a period
 - Use the imperative mood in the subject line
 - Wrap the body at 72 characters
 - Use the body to explain *what* and *why* vs. *how*

Commits aren't required to be GPG signed or signed-off, but it's not discouraged either.

<!-- Links -->
[chris-beams]: https://chris.beams.io/posts/git-commit/
[clang-format]: https://clang.llvm.org/docs/ClangFormat.html
[issues]: https://gitlab.com/bracketttc/ncpty/-/issues
[MIT license]: LICENSE.md
[pull requests]: https://gitlab.com/bracketttc/ncpty/-/merge_requests
[home]: https://gitlab.com/bracketttc/ncpty
[gitter]: https://gitter.im/bracketttc-ncpty/community
