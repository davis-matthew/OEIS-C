/*
 * A369969  Numbers that are not multiples of 5, but their arithmetic
 *          derivative is.
 * https://oeis.org/A369969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 9759, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A369969 – numbers that are not multiples of 5, but whose
 * arithmetic derivative is a multiple of 5.
 *
 * The arithmetic derivative d(n) satisfies:
 *    d(p)   = 1   for prime p
 *    d(ab)  = a·d(b)+b·d(a)
 * In particular, for n = Π pᶜ, we have
 *    d(n) = Σ c·(n/p)
 *
 * This function generates the sequence lazily by testing successive
 * integers and collecting those that meet the conditions.  The
 * implementation uses a small SPF sieve for factorisation.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Configuration – adjust as needed for larger n  ------------------------ */
/* --------------------------------------------------------------------- */
#define MAX_SPF   2000000UL    /* sieve limit – large enough for n ~ 1e4 */
#define MAX_CACHE 200000UL     /* allocated space for cached terms */

/* --------------------------------------------------------------------- */
/* Global tables and state --------------------------------------------- */
/* --------------------------------------------------------------------- */
static uint64_t spf[MAX_SPF + 1];
static bool spf_ready = false;

static seq_t cache[MAX_CACHE];
static size_t cache_len  = 0;
static uint64_t last_checked = 1;

/* --------------------------------------------------------------------- */
/* Helper: initialise SPF sieve --------------------------------------- */
/* --------------------------------------------------------------------- */
static void init_spf(void)
{
    if (spf_ready) return;
    for (uint64_t i = 0; i <= MAX_SPF; ++i) spf[i] = i;
    for (uint64_t i = 2; i * i <= MAX_SPF; ++i)
        if (spf[i] == i)
            for (uint64_t j = i * i; j <= MAX_SPF; j += i)
                if (spf[j] == j) spf[j] = i;
    spf[0] = 0;
    spf[1] = 1;
    spf_ready = true;
}

/* --------------------------------------------------------------------- */
/* Helper: extend cache to contain at least n terms -------------------- */
/* --------------------------------------------------------------------- */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;
    if (cache_len >= n) return;
    init_spf();

    while (cache_len < (size_t)n)
    {
        uint64_t cand = last_checked++;
        if (cand % 5 == 0)   /* exclude multiples of 5 */
            continue;

        uint64_t tmp   = cand;
        uint64_t summ  = 0;   /*  d(n)  */

        /* factorisation via SPF */
        while (tmp > 1)
        {
            uint64_t p = spf[tmp];
            uint32_t e = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++e;
            }
            /* add contribution e * (n / p)   (exact integer) */
            summ += (uint64_t)e * (cand / p);
        }

        if (summ % 5 == 0)
        {
            if (cache_len < MAX_CACHE)
                cache[cache_len++] = (seq_t)cand;
            else
                /* should not happen for required n */
                break;
        }
    }
}

/* --------------------------------------------------------------------- */
/* Required function ----------------------------------------------------- */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* behaviour below offset not specified */
    ensure_cache(n);
    return cache[n - 1];
}
