/*
 * A399091  The maximum exponent in the prime factorization of the cubefree
 *          numbers k such that k+1 is also cubefree.
 * https://oeis.org/A399091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = maximum exponent in the prime factorisation of the
 *            cube‑free number k for which k+1 is also cube‑free (a(1)=0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm precomputes all cube‑free numbers up to a fixed
   limit and collects the required values in an array.  The limit
   is chosen large enough to store at least the first 10 000 terms
   (≈200 000).  The implementation uses an ordinary sieve for
   cube‑free detection and a linear sieve for smallest prime factors. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --- configuration ----------------------------------------------------- */
#define LIMIT 200000          /* numbers we pre‑compute (needs k+1, so +1) */

/* --- static data ------------------------------------------------------- */
static bool cube_free[LIMIT + 2] = { true };     /* n and n+1 cube‑free?   */
static uint8_t max_exp[LIMIT + 2];             /* max exponent of n      */
static uint32_t spf[LIMIT + 2];                /* smallest prime factor   */
static uint32_t primes[180000];               /* enough to hold all primes   */
static seq_t seq[LIMIT];                      /* sequence values          */
static size_t seq_len = 0;                    /* number of stored values   */
static bool ready = false;

/* ------------------------------------------------------------------ */
/* initialise all auxiliary tables                                    */
/* ------------------------------------------------------------------ */
static void init(void)
{
    if (ready) return;
    ready = true;

    /* Smallest prime factor by linear sieve. */
    int pc = 0;
    for (int i = 2; i <= LIMIT + 1; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            primes[pc++] = (uint32_t)i;
        }
        for (int j = 0; j < pc &&
             primes[j] <= spf[i] &&
             (long long)i * primes[j] <= LIMIT + 1; ++j)
            spf[i * primes[j]] = primes[j];
    }

    /* Mark cube‑free numbers.  A number is not cube‑free if
       it is divisible by some p³.  */
    for (int i = 0; i <= LIMIT + 1; ++i) cube_free[i] = true;
    for (int i = 2; (long long)i * i * i <= LIMIT + 1; ++i) {
        if (!cube_free[i]) continue;           /* i itself is cube‑free   */
        long long cube = 1LL * i * i * i;
        for (long long m = cube; m <= LIMIT + 1; m += cube)
            cube_free[(int)m] = false;
    }

    /* Compute the maximum exponent for each cube‑free number (0,1 or 2). */
    for (int n = 1; n <= LIMIT + 1; ++n) {
        if (n == 1) { max_exp[n] = 0; continue; }
        int x = n, exp = 1;
        while (x > 1) {
            int p = (int)spf[x], cnt = 0;
            while (x % p == 0) { ++cnt; x /= p; }
            if (cnt == 2) { exp = 2; break; }
        }
        max_exp[n] = (uint8_t)exp;
    }

    /* Build the sequence: k such that k and k+1 are cube‑free. */
    seq_len = 0;
    for (int k = 1; k <= LIMIT; ++k)
        if (cube_free[k] && cube_free[k + 1])
            seq[seq_len++] = max_exp[k];
}

/* ------------------------------------------------------------------ */
/* public API                                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    init();

    if (n == 0) return 0;                         /* not used, safety */

    if ((idx_t)seq_len < n) return 0;             /* out of bounds */
    return seq[(size_t)(n - 1)];
}
