/*
 * A262976  Number of ordered ways to write n as 2^x + y^2 + pi(z^2) with x >=
 *          0, y >= 0 and z > 0, where pi(m) denotes the number of primes not
 *          exceeding m.
 * https://oeis.org/A262976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A262976 */
/* Number of ordered ways to write n as 2^x + y^2 + pi(z^2)
   with x ≥ 0, y ≥ 0, z > 0.  Computed by sieving pi(z^2)
   for all z whose square is ≤ SIEVE_LIMIT.  The sieve is
   built only once and results reused for subsequent calls. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- configuration ------------------------------------------------- */
/* Sieve limit must cover all z^2 needed for the requested n.
   For n up to ~10000 we need z ≤ 400, so 200 000 is plenty. */
#define SIEVE_LIMIT 200000ULL

/* ---------- static data --------------------------------------------------- */
static bool          primes_built = false;
static bool          *is_prime   = NULL;      /* sieve */
static uint32_t      *pi_count   = NULL;      /* pi(m) for m ≤ SIEVE_LIMIT */

static seq_t        *cnt       = NULL;        /* cnt[p] = #z with pi(z^2)=p */
static idx_t         cnt_size  = 0;           /* capacity of cnt array   */
static idx_t         zmax      = 0;           /* largest z already counted */

/* ---------- sieve -------------------------------------------------------- */
static void init_sieve(void)
{
    if (primes_built) return;
    primes_built = true;

    /* allocate arrays */
    is_prime = (bool*)malloc((SIEVE_LIMIT + 1) * sizeof(bool));
    if (!is_prime) abort();
    pi_count = (uint32_t*)malloc((SIEVE_LIMIT + 1) * sizeof(uint32_t));
    if (!pi_count) abort();

    /* simple Eratosthenes sieve */
    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t p = 2; p * p <= SIEVE_LIMIT; ++p) {
        if (!is_prime[p]) continue;
        for (idx_t j = p * p; j <= SIEVE_LIMIT; j += p)
            is_prime[j] = false;
    }

    /* cumulative prime count */
    uint32_t cum = 0;
    for (idx_t i = 0; i <= SIEVE_LIMIT; ++i) {
        if (is_prime[i]) ++cum;
        pi_count[i] = cum;
    }
}

/* ---------- ensure cnt array up to n ------------------------------------ */
static void ensure_precomputed(idx_t n)
{
    init_sieve();

    /* extend cnt array if necessary */
    if (n + 1 > cnt_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cnt, new_size * sizeof(seq_t));
        if (!tmp) abort();
        /* initialize new entries to zero */
        for (idx_t i = cnt_size; i < new_size; ++i) tmp[i] = 0;
        cnt      = tmp;
        cnt_size = new_size;
    }

    /* count new z values whose pi(z²) ≤ n */
    while (1) {
        idx_t znext = zmax + 1;
        idx_t zsq   = znext * znext;
        if (zsq > SIEVE_LIMIT) break;          /* pre-sieve must be large enough */

        uint32_t pi_val = pi_count[zsq];       /* pi(z²) fits in uint32_t */
        if (pi_val > n) break;                /* beyond needed range */

        cnt[(idx_t)pi_val] = cnt[(idx_t)pi_val] + (seq_t)1;
        zmax = znext;
    }
}

/* ---------- main sequence function --------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset starts at 1, behaviour for 0 irrelevant */

    ensure_precomputed(n);

    seq_t result = 0;

    /* iterate over powers of two 2^x */
    for (idx_t u = 1; u <= n; u <<= 1) {     /* u = 2^x, x ≥ 0 */
        idx_t rem = n - u;                  /* remainder to be written as y^2 + pi(z²) */
        for (idx_t y = 0; ; ++y) {
            idx_t a_val = y * y;            /* y² */
            if (a_val > rem) break;
            idx_t pi_needed = rem - a_val;  /* pi(z²) must equal this */
            result += cnt[pi_needed];
        }
    }

    return result;
}
