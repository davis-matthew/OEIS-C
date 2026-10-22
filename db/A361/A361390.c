/*
 * A361390  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) is carryless n^k base 10.
 * https://oeis.org/A361390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   a(n) = T(i,j) where T(i,j) is carryless i^j base 10. (i,j) are
 *            obtained from index n by antidiagonal enumeration: let d be the
 *            largest integer with d*(d+1)/2 ≤ n; let s = d*(d+1)/2; let
 *            offset = n – s; then i = offset, j = d – offset
 * Range:     u8 n <= 100, u16 n <= 129, u32 n <= 219, u64 n <= 474,
 *            u128 n <= 1234, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* convert a seq_t to base‑10 digits (least significant first).
   The buffer must have at least 200 bytes. */
static void seq_to_digits(seq_t v, uint8_t *buf, size_t *len)
{
    size_t pos = 0;
    if (v == (seq_t)0) {
        buf[0] = 0;
        *len = 1;
        return;
    }
    while (v != (seq_t)0) {
        buf[pos++] = (uint8_t)(v % (seq_t)10);
        v /= (seq_t)10;
    }
    *len = pos;
}

/* carryless multiplication of two seq_t numbers. */
static seq_t carryless_mul(seq_t a, seq_t b)
{
    if (a == (seq_t)0 || b == (seq_t)0) return (seq_t)0;

    uint8_t da[128], db[128], res[256];
    size_t la, lb, lr;

    seq_to_digits(a, da, &la);
    seq_to_digits(b, db, &lb);

    lr = la + lb;
    for (size_t i = 0; i < lr; ++i) res[i] = 0;

    for (size_t i = 0; i < la; ++i) {
        for (size_t j = 0; j < lb; ++j) {
            unsigned temp = (unsigned)res[i + j] + (unsigned)da[i] * (unsigned)db[j];
            res[i + j] = (uint8_t)(temp % 10);      // carryless addition modulo 10
        }
    }

    /* trim leading zeros */
    while (lr > 1 && res[lr - 1] == 0) --lr;

    /* rebuild numeric value */
    seq_t result = (seq_t)0;
    seq_t mul     = (seq_t)1;
    for (size_t i = 0; i < lr; ++i) {
        result += (seq_t)res[i] * mul;
        mul *= (seq_t)10;
    }
    return result;
}

/* integer power with carryless multiplication. */
static seq_t carryless_pow(seq_t base, idx_t exp)
{
    seq_t res = (seq_t)1;
    for (idx_t e = 0; e < exp; ++e)
        res = carryless_mul(res, base);
    return res;
}

/* binary search for diagonal index d: largest d with d*(d+1)/2 ≤ n */
static idx_t diagonal_of(idx_t n)
{
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    idx_t d      = diagonal_of(n);                    /* diagonal index          */
    idx_t start  = d * (d + 1) / 2;                    /* first index of this diag */
    idx_t offset = n - start;                         /* position in diagonal     */
    idx_t i      = offset;                            /* base in exponentiation   */
    idx_t j      = d - offset;                        /* exponent                */

    if (j == 0) return (seq_t)1;                      /* n^0 = 1                */
    if (i == 0) return (seq_t)0;                      /* 0^k = 0 for k>0       */
    return carryless_pow((seq_t)i, j);
}
