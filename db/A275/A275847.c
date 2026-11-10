/*
 * A275847  Permutation of natural numbers: a(0) = 0, a(A153880(n)) =
 *          A255411(a(n)), a(A273670(n)) = A256450(n).
 * https://oeis.org/A275847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(0)=0; for n>=1, if n has no maximal digit in factorial base (n
 *            in A153880): a(n) = A255411(a(A266193(n))), else a(n) =
 *            A256450(A273663(n)) (index of n in A273670)
 * Range:     u8 n <= 119, u16 n <= 5039, u32 n <= 5039, u64 n <= 5039,
 *            u128 n <= 5039, big any n
 * Validated: standard, pass, 105 terms through n = 4783 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* A255411 applied to x: shift factorial-base digits left, increment nonzero digits */
static seq_t A255411_f(seq_t x)
{
    unsigned char dig[80];
    unsigned int k = 0;
    seq_t rem = x;
    while (rem > 0) {
        unsigned int div = k + 2;
        dig[k] = (unsigned char)(long long)(rem % (seq_t)div);
        rem = rem / (seq_t)div;
        ++k;
    }
    seq_t result = 0;
    seq_t fact = 1;
    for (unsigned int i = 1; i <= k + 1; ++i) {
        seq_t digit = 0;
        if (i > 1 && dig[i - 2] != 0)
            digit = (seq_t)dig[i - 2] + 1;
        result += digit * fact;
        fact *= (seq_t)(i + 1);
    }
    return result;
}

/* does x have a maximal digit (digit k at position k) in factorial base */
static bool A275847_has_max(idx_t x)
{
    idx_t k = 1;
    while (x > 0) {
        idx_t d = x % (k + 1);
        if (d == k) return true;
        x /= (k + 1);
        ++k;
    }
    return false;
}

/* does x have a digit 1 in factorial base */
static bool A275847_has_one(idx_t x)
{
    idx_t k = 1;
    while (x > 0) {
        idx_t d = x % (k + 1);
        if (d == 1) return true;
        x /= (k + 1);
        ++k;
    }
    return false;
}

/* inverse of the left shift (A266193): n must be a term of A153880 */
static idx_t A275847_unshift(idx_t n)
{
    idx_t digs[80];
    int cnt = 0;
    idx_t q = n / 2;
    idx_t base = 3;
    while (q > 0) {
        digs[cnt++] = q % base;
        q /= base;
        ++base;
    }
    idx_t m = 0;
    for (int i = cnt - 1; i >= 0; --i)
        m = m * (idx_t)(i + 2) + digs[i];
    return m;
}

static seq_t A275847_rec(idx_t n)
{
    if (n == 0) return 0;
    if (!A275847_has_max(n)) {
        idx_t m = A275847_unshift(n);
        return A255411_f(A275847_rec(m));
    }
    idx_t cnt = 0;
    for (idx_t x = 1; x <= n; ++x)
        if (A275847_has_max(x)) ++cnt;
    idx_t idx = cnt - 1;
    /* idx-th (0-based) number with a digit 1 in factorial base */
    idx_t c = 0, cand = 0;
    for (cand = 1;; ++cand) {
        if (A275847_has_one(cand)) {
            if (c == idx) break;
            ++c;
        }
    }
    return (seq_t)cand;
}

seq_t a(idx_t n)
{
    return A275847_rec(n);
}
