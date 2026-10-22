/*
 * A363025  Sum of divisors of 5*n-2 of form 5*k+2.
 * https://oeis.org/A363025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of divisors of (5*n-2) that are ≡ 2 (mod 5)
 * Range:     u8 n <= 52, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: factor (5*n-2) and generate all its divisors.  Each divisor
   that satisfies d % 5 == 2 is added to the sum.  This runs in
   O(d(m)) time where d(m) is the number of divisors of m,
   which is < 200 for m ≤ 5·10^4.  The algorithm is well below 2 s
   for n up to 10 000. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Forward declaration of the recursive helper that enumerates
   all divisors of a number with given prime factorization. */
static void rec_divisors(const uint64_t *primes, const int *exps,
                         size_t num_factors, size_t idx,
                         uint64_t current, seq_t *sum);

/*
 * Compute a(n) = Σ_{d | (5*n-2), d≡2 (mod 5)} d
 */
seq_t a(idx_t n)
{
    /* m = 5*n - 2   (computed in seq_t to avoid overflow) */
    seq_t m = ((seq_t)5 * n) - (seq_t)2;

    if (m < 2) return (seq_t)0;          /* for safety, although n≥1 */

    /* Factor m.  The factorisation is small (m ≤ 5·10^4 for the
       intended use case), so a simple trial division suffices. */
    uint64_t mm = (uint64_t)m;             /* safe cast for small values */
    uint64_t primes[20];                    /* enough for 64‑bit numbers */
    int     exps[20];
    size_t  cnt = 0;

    uint64_t rem = mm;
    for (uint64_t p = 2; p * p <= rem; ++p) {
        if (rem % p == 0) {
            int e = 0;
            while (rem % p == 0) {
                rem /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt]   = e;
            ++cnt;
            if (cnt >= 20) break;          /* defensive, should not happen */
        }
    }
    if (rem > 1) {
        if (cnt < 20) {
            primes[cnt] = rem;
            exps[cnt]   = 1;
            ++cnt;
        }
    }

    /* Enumerate divisors and aggregate the ones ≡ 2 (mod 5). */
    seq_t sum = (seq_t)0;
    rec_divisors(primes, exps, cnt, 0, (uint64_t)1, &sum);
    return sum;
}

/* Recursive helper that generates all divisors from the prime
   factorisation and adds those congruent to 2 mod 5. */
static void rec_divisors(const uint64_t *primes, const int *exps,
                         size_t num_factors, size_t idx,
                         uint64_t current, seq_t *sum)
{
    if (idx == num_factors) {
        if (current % 5 == 2) {
            *sum += (seq_t)current;
        }
        return;
    }

    uint64_t p   = primes[idx];
    int     exp  = exps[idx];
    uint64_t val = 1;

    for (int e = 0; e <= exp; ++e) {
        rec_divisors(primes, exps, num_factors, idx + 1,
                     current * val, sum);
        val *= p;
    }
}
