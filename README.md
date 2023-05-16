# num2shu - convert numbers to Chinese numerals

A versatile tool to convert numbers into various Chinese numeral representations and formats.

Numbers must be under 1 quindecillion (10<sup>48</sup>).

Decimal numbers supported.

## Installation

`make && make install`

## Usage

`num2shu [-htfznsr] <number>`

Nondigit characters will be stripped except for a single leading `-` for negative numbers (see Note 2).

## Options

Examples with argument of 21203456789.

Without options: 二百一十二亿零三百四十五万六千七百八十九

| Short          | Long            | Description                                                      | Example
| -------------- | --------------- | ---------------------------------------------------------------- | ----------------------------------------
| `-h`           | `--help`        | Show this help message and exit.                                 |
| `-t`           | `--traditional` | Use traditional numerals and units.                              | 二百一十二億零三百四十五萬六千七百八十九
| `-f`           | `--financial`   | Use financial numerals and units. (Overrides `-z` and `-n`)      | 贰佰壹拾贰億零叁佰肆拾伍萬陆仟柒佰捌拾玖
| `-z`           | `--zero`        | Use the alternative "zero" character (〇).                       | 二百一十二亿〇三百四十五万六千七百八十九
| `-n`           | `--natural`     | Use "两" for "two" in certain contexts.                          | 两百一十二亿零三百四十五万六千七百八十九
| `-s`           | `--simple`      | No magnitudes, just simple numeral replacement. (Overrides `-n`) | 二一二零三四五六七八九
| `-r`           | `--radio`       | Like `-s`, but use unambiguous radio numerals. (Overrides `z`)   | 两幺两洞三四五六拐八勾

### Notes:
1. These options combine generally, e.g.: if both `-t` and `-f` are passed, traditional financial numerals will be used.
2. To convert negative numbers, prepend with the end-of-options marker `--`, e.g.: `num2shu -t -- -88`

## Development

Debug builds will be generated when the environment variable `$BUILD` is set to `debug`.

Add test cases to `test/num2shu_test_cases.c` and run with `make test`.

