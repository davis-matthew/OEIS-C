/*
 * A261906  Numbers that are the sum of two nonzero palindromes.
 * https://oeis.org/A261906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = first n integers that can be written as the sum of two
 *            non‑zero decimal palindromes. The implementation generates
 *            palindromes incrementally and checks for each candidate whether
 *            a decomposition exists
 * Range:     u8 n <= 110, u16 n <= 110, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Helpers:   is_palindrome
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_palindrome.h"   /* provides is_palindrome(seq_t,idx_t) */

/* ---------- palindrome cache ---------- */
static seq_t   *_pal      = NULL;      /* list of palindromic numbers */
static size_t  _pal_cap   = 0;         /* allocated size of _pal */
static size_t  _pal_len   = 0;         /* number of palindromes stored */
static seq_t   _pal_max   = 0;         /* largest number checked for palindromicity */

/* ensure palindromes up to 'limit' are present in _pal */
static void _ensure_pal_limit(seq_t limit)
{
    if (limit <= _pal_max) return;          /* already up to date */

    /* grow capacity if needed */
    if (_pal_cap == 0) _pal_cap = 8;
    while (_pal_cap <= (size_t)(limit + 1)) _pal_cap <<= 1;

    _pal = (seq_t *)realloc(_pal, _pal_cap * sizeof(seq_t));

    /* compute palindromes from _pal_max + 1 to limit */
    for (seq_t x = _pal_max + 1; x <= limit; ++x) {
        if (is_palindrome(x, 10)) {
            if (_pal_len == _pal_cap) { /* enlarge list if needed */
                _pal_cap <<= 1;
                _pal = (seq_t *)realloc(_pal, _pal_cap * sizeof(seq_t));
            }
            _pal[_pal_len++] = x;
        }
    }
    _pal_max = limit;
}

/* test whether x is a sum of two non‑zero palindromes */
static bool _is_sum_of_two_nonzero_palindromes(seq_t x)
{
    for (size_t i = 0; i < _pal_len && _pal[i] < x; ++i) {
        seq_t p   = _pal[i];
        seq_t rest = x - p;
        if (rest > 0 && is_palindrome(rest, 10)) return true;
    }
    return false;
}

/* ---------- sequence cache ---------- */
static seq_t   *_seq      = NULL;   /* array of discovered terms */
static size_t  _seq_cap   = 0;      /* allocated capacity of _seq */
static size_t  _seq_len   = 0;      /* number of terms stored */
static seq_t   _current   = 0;      /* largest integer examined so far */

/* generate sequence terms until we have at least n terms */
static void _generate_up_to_request(idx_t n)
{
    while (_seq_len < (size_t)n) {
        ++_current;
        _ensure_pal_limit(_current);

        if (_is_sum_of_two_nonzero_palindromes(_current)) {
            if (_seq_len == _seq_cap) {   /* enlarge array if needed */
                _seq_cap = (_seq_cap == 0) ? 8 : _seq_cap << 1;
                _seq = (seq_t *)realloc(_seq, _seq_cap * sizeof(seq_t));
            }
            _seq[_seq_len++] = _current;
        }
    }
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for offset 0, but safe */
    _generate_up_to_request(n);
    return _seq[n - 1];
}
