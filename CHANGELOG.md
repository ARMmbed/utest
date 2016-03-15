# Change Log
All notable changes to this project will be documented in this file.
This project adheres to [Semantic Versioning](http://semver.org/).

## [Unreleased]
## [1.10.1] - 2016-03-15

### Added
- Coverage configuration file.

### Fixed
- Removed duplicate dependency.

## [1.10.0] - 2016-03-02

### Added
- Support for `greentea-client` APIs: preamble, hand-shake and test suite setup.
- Support for `Greentea` automation tool: Additional KiVi (key-value) prints.
- Separate Greentea handlers section.
- Excluded lambdas (C++11) code failing to compile on ARMCCv5.

## [1.9.2] - 2016-02-29
### Fixed
- Calling `raise_failure` before harness is initialized won't have an effect.

## [1.9.1] - 2016-01-25
### Fixed
- Also check `control_handler` in `Case::is_empty()`.

## [1.9.0] - 2016-01-07
### Added
- Location information to `failure_t` and report it in failure handlers.
- Added documentation about failure reasons and locations to Readme.

### Fixed
- Assertion failures on test setup and test teardown will now not be ignored.

## [1.8.2] - 2015-12-23
### Added
- Code examples from Readme as tests.

### Changed
- Updated Readme with more documentation.

## [1.8.1] - 2015-12-22
### Added
- A few more test cases for case repeat counting.
- Self test for case timeout handling.
- Self test for case teardown failure.
- Self test for callback validation (with attributes).

### Changed
- Rename `case_control_handler` test to `case_control_repeat`.

### Fixed
- Modifying case attributes during callback validation.

## [1.8.0] - 2015-12-21
### Added
- Self test for case setup failure.
- Self test for case handler repeat counting.
- Explicit constants of each handler type to `{ignore|default}_handler`.
- Case attribute modification in `validate_callback`.

### Fixed
- Call count in example in Readme.

### Changed
- Replace `repeat_count` with `call_count`, which starts at 1 not at 0.

## [1.7.4] - 2015-12-17
### Added
- Added `CaseNoRepeat` and `CaseNoTimeout` aliases.

### Fixed
- Arbitration behavior when adding `CaseNoTimeout` to `CaseRepeat{All|Handler}OnTimeout`.

## [1.7.3] - 2015-12-16
### Fixed
- Order of unity macro arguments. They were backwards before.

## [1.7.2] - 2015-12-16
### Added
- Unit test for asserting `control_t` arbitration behavior.

### Fixed
- Clarify and implement `control_t` arbitration behavior.

## [1.7.1] - 2015-12-15
### Added
- This changelog  }:-o

## [1.7.0] - 2015-12-15
### Added
- `CaseAwait` control modifier and logic: Makes a case wait without timeout until a callback is validated.
- Explicit `TIMEOUT_*` enumeration for the magic values.

### Changed
- `CaseTimeout` and `CaseRepeat{All|Handler}OnTimeout` modifiers are now constructed using factory functions, rather than inheritance. This allows the compiler to infer the correct return type for the ternary operator.

## [1.6.0] - 2015-12-11
### Added
- Missing implementations for all callback handlers for all (especially the greentea) behaviors. These implementations only forward to the verbose handlers, however, should we modify the reporting style in the future, tests using the behavior-specific handlers will not have to be adapted.

### Changed
- Updated examples to call the behavior-specific handlers for reporting.

## [1.5.1] - 2015-12-10
### Added
- Self test for setup failure.

### Fixed
- Bugs in repeat logic.
- Bugs in index counting.
- Allow repeat on timeout and normal repeat at the same time.

## [1.5.0] - 2015-12-10
### Added
- Callback hook for unity's ignore handler.

## [1.4.0] - 2015-12-10
### Added
- `CaseRepeat{All|Handler}OnTimeout`: allows to repeat the test case automatically on timeout.
- Stringify for `status_t`.
- Allow ignoring of any failures using modifier mask.
- Global specification failure handler, which is called anytime a failure occurs, even on setup and teardown of specifications. This also allows for self testing using unity macros, as it prevents recursive case failure handler calling (think of a failed assert in the case failure handler).

### Changed
- deprecate `CaseRepeat` for `CaseRepeatAll` and `CaseRepeatHandlerOnly` for `CaseRepeatHandler`.

## [1.3.0] - 2015-12-08
### Changed
- Do not need to execute handlers atomically anymore. This interfered with test cases that required polling of interrupts.

## [1.2.0] - 2015-12-08
### Changed
- `Harness::run` returns whether the specification is valid and is being run now
- After specification teardown harness does not die anymore.

## [1.1.0] - 2015-12-04
### Changed
- `Harness::run` allows starting from a different test case than zero. This can be used

## [1.0.1] - 2015-11-25
### Changed
- Updated unity module test dependency to v2.0.0

## 1.0.0 - 2015-11-25
### Added
- v1 of this small test harness. See Readme.

[Unreleased]: https://github.com/ARMmbed/utest/compare/v1.10.1...HEAD
[1.10.1]: https://github.com/ARMmbed/utest/compare/v1.10.0...v1.10.1
[1.10.0]: https://github.com/ARMmbed/utest/compare/v1.9.2...v1.10.0
[1.9.2]: https://github.com/ARMmbed/utest/compare/v1.9.1...v1.9.2
[1.9.1]: https://github.com/ARMmbed/utest/compare/v1.9.0...v1.9.1
[1.9.0]: https://github.com/ARMmbed/utest/compare/v1.8.2...v1.9.0
[1.8.2]: https://github.com/ARMmbed/utest/compare/v1.8.1...v1.8.2
[1.8.1]: https://github.com/ARMmbed/utest/compare/v1.8.0...v1.8.1
[1.8.0]: https://github.com/ARMmbed/utest/compare/v1.7.4...v1.8.0
[1.7.4]: https://github.com/ARMmbed/utest/compare/v1.7.3...v1.7.4
[1.7.3]: https://github.com/ARMmbed/utest/compare/v1.7.2...v1.7.3
[1.7.2]: https://github.com/ARMmbed/utest/compare/v1.7.1...v1.7.2
[1.7.1]: https://github.com/ARMmbed/utest/compare/v1.7.0...v1.7.1
[1.7.0]: https://github.com/ARMmbed/utest/compare/v1.6.0...v1.7.0
[1.6.0]: https://github.com/ARMmbed/utest/compare/v1.5.1...v1.6.0
[1.5.1]: https://github.com/ARMmbed/utest/compare/v1.5.0...v1.5.1
[1.5.0]: https://github.com/ARMmbed/utest/compare/v1.4.0...v1.5.0
[1.4.0]: https://github.com/ARMmbed/utest/compare/v1.3.0...v1.4.0
[1.3.0]: https://github.com/ARMmbed/utest/compare/v1.2.0...v1.3.0
[1.2.0]: https://github.com/ARMmbed/utest/compare/v1.1.0...v1.2.0
[1.1.0]: https://github.com/ARMmbed/utest/compare/v1.0.1...v1.1.0
[1.0.1]: https://github.com/ARMmbed/utest/compare/v1.0.0...v1.0.1
