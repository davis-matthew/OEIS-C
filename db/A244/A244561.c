/*
 * A244561  Odd integers m such that for every integer k > 0, m*2^k+1 has a
 *          divisor in the set {3, 5, 7, 13, 17, 241}.
 * https://oeis.org/A244561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-48) + 11184810 for n > 48. For 1 ≤ n ≤ 48, a(n) is
 *            the first 48 odd integers m such that for every integer k > 0
 *            the number m·2^k + 1 is divisible by at least one of the primes
 *            {3, 5, 7, 13, 17, 241}
 * Range:     u8 none, u16 none, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Prime list used in the definition */
static const uint32_t primes[6] = {3, 5, 7, 13, 17, 241};

/* Storage for the first 48 terms */
static seq_t first48[48];
static bool prepared = false;

/* Pre‑compute the first 48 terms and store them */
static void prepare(void)
{
    if (prepared) return;

    /* Pre‑compute 2^k (mod p) for k = 0 … 24, for every prime p */
    uint32_t pow2[6][25];
    for (int pi = 0; pi < 6; ++pi) {
        uint32_t p = primes[pi];
        pow2[pi][0] = 1 % p;
        for (int k = 1; k <= 24; ++k) {
            pow2[pi][k] = (uint32_t)((pow2[pi][k - 1] * 2ULL) % p);
        }
    }

    /* Enumerate odd m until 48 solutions are found */
    uint32_t count = 0;
    uint64_t m = 1;              /* 64‑bit loop counter – values fit easily */
    while (count < 48) {
        if ((m & 1ULL) == 0) {   /* skip even numbers */
            m += 2;
            continue;
        }
        bool ok = true;
        for (int k = 1; k <= 24 && ok; ++k) {
            bool found = false;
            for (int pi = 0; pi < 6; ++pi) {
                uint32_t p = primes[pi];
                uint32_t term = (uint32_t)(((m % p) * pow2[pi][k]) % p + 1);
                if (term % p == 0) {
                    found = true;
                    break;
                }
            }
            if (!found) ok = false;
        }
        if (ok) {
            first48[count++] = (seq_t)m;
        }
        m += 2;                  /* next odd candidate */
    }
    prepared = true;
}

seq_t a(idx_t n)
{
    prepare();                       /* ensure the first 48 terms are ready */
    if (n == 0) return 0;            /* undefined, but harmless */

    idx_t idx    = n - 1;            /* 0‑based index */
    idx_t block  = idx / 48;         /* how many full periods have passed */
    idx_t offset = idx % 48;         /* position within the first period */
    seq_t inc    = (seq_t)11184810ULL;   /* 2 × product of the primes */

    return first48[offset] + (seq_t)block * inc;
}
