/*
 * A288172  a(n) = smallest number that is the sum of 2n - 1, 2n + 1, and 2n +
 *          3 consecutive primes.
 * https://oeis.org/A288172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest number that is the sum of 2n‑1, 2n+1, and 2n+3
 *            consecutive primes
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Types provided by the build */
extern seq_t a(idx_t n);

#define SIEVE_LIMIT 20000000  /* 20 million, adjust if more needed */

static bool sieve_done = false;
static seq_t *primes = NULL;       /* primes[0..prime_count-1] */
static idx_t prime_count = 0;
static seq_t *cum = NULL;          /* cumulative sums, size prime_count+1 */

/* ------------------------------------------------------------------ */
/* Build primes up to SIEVE_LIMIT and compute cumulative sums.       */
/* ------------------------------------------------------------------ */
static void build_primes_and_cum(void)
{
    if (sieve_done) return;

    /* Allocate temporary boolean array for sieve */
    uint8_t *is_composite = (uint8_t*)malloc(SIEVE_LIMIT + 1);
    if (!is_composite) exit(1);
    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i) is_composite[i] = 0;

    /* Reserve space for primes */
    size_t reserve = SIEVE_LIMIT / 10; /* roughly pi(20M) ~ 1.2M */
    primes = (seq_t*)malloc(reserve * sizeof(seq_t));
    if (!primes) exit(1);

    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            /* i is prime */
            if (prime_count == reserve) {
                reserve *= 2;
                primes = (seq_t*)realloc(primes, reserve * sizeof(seq_t));
                if (!primes) exit(1);
            }
            primes[prime_count++] = (seq_t)i;

            /* Mark multiples */
            if ((i * i) <= SIEVE_LIMIT) {
                for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                    is_composite[j] = 1;
            }
        }
    }
    free(is_composite);

    /* Allocate cumulative sum array */
    cum = (seq_t*)malloc((prime_count + 1) * sizeof(seq_t));
    if (!cum) exit(1);
    cum[0] = 0;
    for (idx_t i = 0; i < prime_count; ++i)
        cum[i + 1] = cum[i] + primes[i];

    sieve_done = true;
}

/* ------------------------------------------------------------------ */
/* Main function: compute a(n) for n >= 1.                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* not in OEIS range, but safe */
    build_primes_and_cum();          /* ensure primes ready */

    idx_t L1 = 2 * n - 1;            /* 2n-1 */
    idx_t L2 = 2 * n + 1;            /* 2n+1 */
    idx_t L3 = 2 * n + 3;            /* 2n+3 */

    /* Check that we have enough primes; if not, we cannot answer */
    if (L3 > prime_count) return 0;  /* fallback: matrix not big enough */

    idx_t i1 = 0, i2 = 0, i3 = 0;
    while (true) {
        if (i1 + L1 > prime_count || i2 + L2 > prime_count || i3 + L3 > prime_count)
            return 0;  /* should not happen with chosen sieve size */

        seq_t s1 = cum[i1 + L1] - cum[i1];
        seq_t s2 = cum[i2 + L2] - cum[i2];
        seq_t s3 = cum[i3 + L3] - cum[i3];

        if (s1 == s2 && s1 == s3)
            return s1;

        /* Increment the pointer that holds the smallest sum */
        if (s1 <= s2 && s1 <= s3) {
            ++i1;
        } else if (s2 <= s1 && s2 <= s3) {
            ++i2;
        } else {
            ++i3;
        }
    }
}
