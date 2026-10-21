/*
 * A243047  Numbers n such that k*n/(k-n) and k*n/(k+n) are both never
 *          integers for k > 0.
 * https://oeis.org/A243047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A243047
 * This function returns the nth term of the sequence.
 * A number n is a term if there is no integer k>0 such that
 * both k*n/(k-n) and k*n/(k+n) are integers.
 * The condition is equivalent to: n^2 has no two divisors whose difference is 2*n.
 * The function checks each candidate n in order, counting valid numbers
 * until the requested position is reached.
 */

#include <stdbool.h>

/* Forward declaration of the recursive divisor generator */
static void rec_div(idx_t idx, seq_t cur, idx_t pf_cnt, const idx_t *pf,
                    const idx_t *exp2, seq_t n_sq, idx_t n, bool *found);

/* Return true if the given n is invalid (i.e., there exists k>0 with both
 * fractions integer). */
static bool has_k(idx_t n)
{
    seq_t n_sq = (seq_t)n * (seq_t)n;
    idx_t pf[16], exp[16];
    idx_t cnt = 0;
    idx_t m = n;

    /* Factorisation of n (trial division) */
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            pf[cnt] = p;
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            exp[cnt] = e;
            ++cnt;
        }
    }
    if (m > 1) {
        pf[cnt] = m;
        exp[cnt] = 1;
        ++cnt;
    }

    /* Prepare exponent array for n^2 (double the exponents) */
    idx_t exp2[16];
    for (idx_t i = 0; i < cnt; ++i) exp2[i] = exp[i] * 2;

    bool found = false;
    rec_div(0, (seq_t)1, cnt, pf, exp2, n_sq, n, &found);
    return found;
}

/* Recursive generation of all divisors of n^2.  At each leaf we test
 * whether the divisor a satisfies the "bad" condition. */
static void rec_div(idx_t idx, seq_t cur, idx_t pf_cnt, const idx_t *pf,
                    const idx_t *exp2, seq_t n_sq, idx_t n, bool *found)
{
    if (*found) return;

    if (idx == pf_cnt) {
        seq_t a = cur;
        seq_t two_n = (seq_t)2 * (seq_t)n;

        /* Check a + 2n divides n^2 */
        seq_t b1 = a + two_n;
        if (n_sq % b1 == 0) {
            *found = true;
            return;
        }

        /* Check 2n - a divides n^2, only if a < n (corresponds to k < n) */
        if (a < (seq_t)n) {
            seq_t b2 = two_n - a;
            if (n_sq % b2 == 0) {
                *found = true;
                return;
            }
        }
        return;
    }

    seq_t mult = (seq_t)1;
    for (idx_t e = 0; e <= exp2[idx]; ++e) {
        rec_div(idx + 1, cur * mult, pf_cnt, pf, exp2, n_sq, n, found);
        mult *= pf[idx];
    }
}

/* Public function to compute the nth term of the sequence. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    for (idx_t k = 1;; ++k) {
        if (!has_k(k)) {
            ++found;
            if (found == n) return (seq_t)k;
        }
    }
}
