/*
 * A308763  a(n) = Sum_{d|n} d^(n-2).
 * https://oeis.org/A308763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(n-2)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/*--------------------------------------------------------------------
 * Helper: exponentiation by squaring.
 * Works for any seq_t that supports *, + and integer literals.
 *--------------------------------------------------------------------*/
static seq_t power(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/*--------------------------------------------------------------------
 * Recursive enumeration of all divisors of n.
 *--------------------------------------------------------------------*/
static void rec_sum(idx_t idx, seq_t curr,
                    const uint64_t *primes, const uint64_t *exps,
                    size_t cnt, idx_t exp, seq_t *sum)
{
    if (idx == cnt) {
        *sum += power(curr, exp);
        return;
    }
    seq_t val = curr;
    for (int e = 0; e <= (int)exps[idx]; ++e) {
        rec_sum(idx + 1, val, primes, exps, cnt, exp, sum);
        val *= (seq_t)primes[idx];
    }
}

/*--------------------------------------------------------------------
 * Main sequence function.
 *--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Handle the special case n = 1 directly. */
    if (n == (idx_t)1)
        return (seq_t)1;

    /* Exponent for the power term: n-2. */
    idx_t exp = n - 2;

    /* Factorisation of n.  Results are stored in small arrays. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt = 0;

    uint64_t temp = n;

    /* Factor out 2. */
    if ((temp & 1) == 0) {
        uint64_t e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;  /* divide by 2 */
            ++e;
        }
        primes[cnt] = 2;
        exps[cnt]   = e;
        ++cnt;
    }

    /* Factor odd primes up to sqrt(temp). */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint64_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
        }
    }

    /* Remaining prime factor > sqrt(n). */
    if (temp > 1) {
        primes[cnt] = temp;
        exps[cnt]   = 1;
        ++cnt;
    }

    /* Sum over all divisors. */
    seq_t sum = (seq_t)0;
    rec_sum((idx_t)0, (seq_t)1, primes, exps, cnt, exp, &sum);

    return sum;
}
