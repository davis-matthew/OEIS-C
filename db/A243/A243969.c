/*
 * A243969  Integers j not of form 3m+2 such that for any integer k > 0,
 *          j*10^k+1 has a divisor in the set { 7, 11, 13, 37 }.
 * https://oeis.org/A243969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-24) + 111111 (n > 24) Initial values are computed by
 *            searching for the smallest integers j that satisfy the covering
 *            set condition listed in the OEIS entry. For each j, j % 3 != 2
 *            and for k = 1..6 (the period of 10^k modulo the covering primes
 *            7, 11, 13, 37) there exists a prime p in t...
 * Range:     u8 none, u16 n <= 15, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* The type definitions seq_t and idx_t are supplied by the build. */

static seq_t base[24];
static bool initialized = false;

static void compute_base(void)
{
    if (initialized) return;

    /* primes of the covering set */
    const int PRIMES[4] = {7, 11, 13, 37};
    /* precompute 10^r mod p for r = 1..6 */
    static int pow10_mod[4][7];
    for (int p = 0; p < 4; ++p) {
        int mod = 10 % PRIMES[p];
        pow10_mod[p][1] = mod;
        for (int r = 2; r <= 6; ++r) {
            mod = (mod * 10) % PRIMES[p];
            pow10_mod[p][r] = mod;
        }
    }

    unsigned long long j = 1;
    int count = 0;
    while (count < 24) {
        if (j % 3 == 2) {  /* skip j of the form 3m+2 */
            ++j;
            continue;
        }

        bool ok = true;
        for (int r = 1; r <= 6 && ok; ++r) {
            bool found = false;
            for (int p = 0; p < 4; ++p) {
                int mod = (int)(((j % PRIMES[p]) * (unsigned int)pow10_mod[p][r]) % PRIMES[p]);
                mod = (mod + 1) % PRIMES[p];
                if (mod == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) ok = false;
        }

        if (ok) {
            base[count++] = (seq_t)j;
        }
        ++j;
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    /* The sequence is 1-indexed.  The recurrence holds for all n >= 1. */
    compute_base();
    if (n == 0) return 0;  /* undefined, but guard against it */

    idx_t block = (n - 1) / 24;
    int offset = (int)((n - 1) % 24);
    seq_t term = base[offset] + ((seq_t)111111) * (seq_t)block;
    return term;
}
