/*
 * A344752  Quotient A306927(k) / A344705(k) computed for such k >= 1 that the
 *          quotient is a nonnegative natural number. (The k are given by
 *          A344700.)
 * https://oeis.org/A344752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A306927(k)/A344705(k) where k runs through A344700, the
 *            k>=1 for which A344705(k) divides A306927(k) with a nonnegative
 *            quotient. Found by a linear search over k. A306927(k) = psi(k) -
 *            k and A344705(k) = k + psi(k) - sigma(k), with psi the Dedekind
 *            psi function
 * Range:     u8 n <= 22, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Validated: partial, pass, 82 terms through n = 82 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef unsigned __int128 A344752_u128;

static uint64_t A344752_terms[4096];
static size_t A344752_count = 0;
static uint64_t A344752_next_k = 1;

static bool A344752_test(uint64_t k, uint64_t *q)
{
    if (k == 1) { *q = 0; return true; }
    uint64_t tmp = k;
    A344752_u128 psi = k;
    A344752_u128 sigma = 1;
    for (uint64_t p = 2; p * p <= tmp; p += (p == 2 ? 1 : 2)) {
        if (tmp % p == 0) {
            A344752_u128 pw = 1, s = 1;
            while (tmp % p == 0) {
                tmp /= p;
                pw *= p;
                s += pw;
            }
            psi = psi / p * (p + 1);
            sigma *= s;
        }
    }
    if (tmp > 1) {
        psi = psi / tmp * (tmp + 1);
        sigma *= (A344752_u128)tmp + 1;
    }
    A344752_u128 sum = (A344752_u128)k + psi;
    if (sum <= sigma) return false;
    A344752_u128 D = sum - sigma;
    A344752_u128 N = psi - k;
    if (N % D != 0) return false;
    *q = (uint64_t)(N / D);
    return true;
}

seq_t a(idx_t n)
{
    while (A344752_count < (size_t)n && A344752_count < 4096) {
        uint64_t q;
        if (A344752_test(A344752_next_k, &q)) {
            A344752_terms[A344752_count++] = q;
        }
        A344752_next_k++;
    }
    return (seq_t)A344752_terms[n - 1];
}
