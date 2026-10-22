/*
 * A329170  Numbers of the form k^2 + 2 that are the sums of two squares.
 * https://oeis.org/A329170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A329170: numbers of the form k^2 + 2 that are the sum of two squares.
   Raw formula: a(n) = number x such that x = k^2 + 2 and x = u^2 + v^2 for some integers u,v. 
   Computation: iterate even k, test each k^2+2 via the sum‑of‑two‑squares theorem. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- prime sieve --------------------------------------------------- */
static uint32_t *primes = NULL;   /* list of primes */
static size_t   primes_len = 0;   /* number of primes stored   */
static size_t   primes_cap = 0;   /* allocated capacity       */
static bool     primes_sieved = false;

static void ensure_primes_capacity(size_t needed)
{
    if (primes_cap >= needed) return;
    size_t new_cap = primes_cap ? primes_cap << 1 : 64;
    while (new_cap < needed) new_cap <<= 1;
    uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
    if (!tmp) exit(1);
    primes = tmp;
    primes_cap = new_cap;
}

static void init_primes(void)
{
    if (primes_sieved) return;
    /* 2 M is ample for k up to the first 10 000 terms of this sequence. */
    const uint32_t LIMIT = 2000000u;
    uint8_t *is_composite = (uint8_t *)calloc(LIMIT + 1, 1);
    if (!is_composite) exit(1);

    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!is_composite[i]) {
            ensure_primes_capacity(primes_len + 1);
            primes[primes_len++] = i;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint32_t j = i * i; j <= LIMIT; j += i) is_composite[j] = 1;
            }
        }
    }
    free(is_composite);
    primes_sieved = true;
}

/* ---------- test for sum of two squares --------------------------------- */
static bool is_sum_of_two_squares(seq_t val)
{
    uint64_t m = (uint64_t)val;      /* value must fit in 64 bits for this problem */
    uint64_t tmp = m;
    for (size_t i = 0; i < primes_len && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        int parity = 0;
        while (tmp % p == 0) { tmp /= p; parity ^= 1; }
        if (parity && (p % 4) == 3) return false;   /* p ≡ 3 (mod 4) with odd exponent */
    }
    if (tmp > 1 && (tmp % 4) == 3) return false;     /* remaining prime factor 3 mod 4 */
    return true;
}

/* ---------- main sequence function -------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n < 1, return 0 as safe default */

    init_primes();                     /* ensure prime list for factorisation */

    idx_t count = 0;
    uint64_t k = 0;                    /* even k, start from 0 */

    while (true) {
        /* compute k^2 + 2, using 128‑bit to avoid overflow for very large k */
        uint64_t m64 = (uint64_t)((__int128)k * k + 2);
        seq_t  m   = (seq_t)m64;

        if (is_sum_of_two_squares(m)) {
            ++count;
            if (count == n) return m;  /* found the n-th term */
        }
        k += 2;                       /* next even k */
    }
}
