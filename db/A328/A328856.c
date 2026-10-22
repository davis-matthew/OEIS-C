/*
 * A328856  Number of factorizations of n into distinct numbers with an odd
 *          number of distinct prime factors.
 * https://oeis.org/A328856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9041, u16 n <= 9041, u32 n <= 9041, u64 n <= 9041,
 *            u128 n <= 9041, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A328856:
   a(n) = number of ways to write n as a product of distinct
   integers > 1, each factor having an odd number of distinct
   prime factors.  The implementation uses recursive enumeration
   together with a small sieve for factorisation. */

#include <stdint.h>
#include <stdlib.h>

/* static data for smallest prime factor and odd‑prime flag */
static idx_t spf_max = 0;
static uint64_t *spf = NULL;          /* smallest prime factor array */
static uint8_t *odd_flag = NULL;      /* 1 if number has odd omega, else 0 */

/* initialise the sieve up to 'limit' (inclusive) */
static void init_sieve(idx_t limit)
{
    if (limit <= spf_max) return;           /* already computed */

    /* reallocate arrays */
    free(spf);
    free(odd_flag);
    spf = (uint64_t*)malloc((limit + 1) * sizeof(uint64_t));
    odd_flag = (uint8_t*)malloc((limit + 1) * sizeof(uint8_t));
    if (!spf || !odd_flag) return;          /* allocation failure safety */

    /* initialise smallest prime factors */
    spf[0] = spf[1] = 0;
    for (idx_t i = 2; i <= limit; ++i) spf[i] = i;

    /* classic sieve for smallest prime factor */
    for (idx_t i = 2; (idx_t)i * (idx_t)i <= limit; ++i) {
        if (spf[i] == i) {   /* i is prime */
            for (idx_t j = i * i; j <= limit; j += i) {
                if (spf[j] == j) spf[j] = i;
            }
        }
    }

    /* compute odd‑omega flag */
    odd_flag[0] = odd_flag[1] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        idx_t x = i;
        int parity = 0;
        while (x > 1) {
            uint64_t p = spf[x];
            parity ^= 1;         /* toggle parity for each distinct prime */
            while (x % p == 0) x /= p;
        }
        odd_flag[i] = (uint8_t)parity;
    }

    spf_max = limit;
}

/* recursive count of factorizations of 'n' with factors <= 'max' */
static seq_t rec_count(idx_t n, idx_t max)
{
    if (n == 1) return 1;          /* empty factor set */

    seq_t total = 0;
    for (idx_t d = max; d >= 2; --d) {
        if (n % d != 0) continue;
        if (!odd_flag[d]) continue;        /* factor must have odd prime count */
        total += rec_count(n / d, d - 1);
    }
    return total;
}

/* public function computing a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but safe guard */

    init_sieve(n);                  /* ensure sieve data covers n */
    return rec_count(n, n);
}
