/*
 * A342781  Positions of words in A342753 in which 1st digit != last digit.
 * https://oeis.org/A342781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th k such that first digit of word w(k) != last digit
 *            of w(k), where w(k) is the k-th word of A342753 (first digit =
 *            [3|k], last digit = [3|m] for the chain end m reached via k ->
 *            s'(k) or t'(k) until 0; computed by search)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int A342781_first_last_differ(idx_t k)
{
    int first = (k % 3 == 0) ? 1 : 0;
    idx_t cur = k;
    for (;;) {
        idx_t a = cur - 1;
        idx_t tp = a / 3;
        idx_t sp = a - tp;
        idx_t child = (cur % 3 == 0) ? tp : sp;
        if (child == 0) break;
        cur = child;
    }
    int last = (cur % 3 == 0) ? 1 : 0;
    return first != last;
}

static idx_t A342781_cache_n = 0;   /* count of terms found so far */
static idx_t A342781_cache_k = 0;   /* last k examined */
static idx_t A342781_cache_v = 0;   /* value of term number cache_n */

seq_t a(idx_t n)
{
    idx_t cnt, k;
    if (A342781_cache_n != 0 && A342781_cache_n <= n) {
        cnt = A342781_cache_n;
        k = A342781_cache_k;
        if (cnt == n) return (seq_t)A342781_cache_v;
    } else {
        cnt = 0;
        k = 0;
    }
    while (cnt < n) {
        ++k;
        if (A342781_first_last_differ(k)) ++cnt;
    }
    A342781_cache_n = cnt;
    A342781_cache_k = k;
    A342781_cache_v = k;
    return (seq_t)k;
}
