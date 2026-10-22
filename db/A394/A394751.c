/*
 * A394751  Numbers k such that sigma(k) = psi(k) + tau(k) + Omega(k)^4.
 * https://oeis.org/A394751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A394751
   Numbers k such that sigma(k) = psi(k) + tau(k) + Omega(k)^4.
   Sequence = {5375, 397535} ∪ {546 * p^2 | p prime ∉ {2,3,7,13}}.
   Terms are listed in increasing order. */

/*--- required includes ---------------------------------------*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*--- static storage for prime generation -----------------------*/
static uint64_t *all_primes      = NULL;
static size_t   all_primes_cnt   = 0;
static size_t   all_primes_cap   = 0;

static uint64_t *series_primes   = NULL;  /* primes not dividing 546 */
static size_t   series_primes_cnt = 0;
static size_t   series_primes_cap = 0;

static uint64_t prime_candidate  = 2;      /* next number to test */

/*--- helper: grow an array ------------------------------*/
static void grow_uint64_array(uint64_t **arr, size_t *cnt, size_t *cap, size_t need)
{
    if (*cap >= need) return;
    size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
    if (newcap < need) newcap = need;
    uint64_t *tmp = (uint64_t *)realloc(*arr, newcap * sizeof(uint64_t));
    if (!tmp) { abort(); }
    *arr = tmp;
    *cap = newcap;
}

/*--- helper: ensure we have at least 'k' series primes --------*/
static void ensure_series_primes(size_t k)
{
    while (series_primes_cnt < k)
    {
        /* test candidate for primality */
        bool is_prime = true;
        for (size_t i = 0; i < all_primes_cnt; ++i)
        {
            uint64_t p = all_primes[i];
            if (p * p > prime_candidate) break;
            if (prime_candidate % p == 0)
            {
                is_prime = false;
                break;
            }
        }

        if (is_prime)
        {
            /* store in all_primes */
            grow_uint64_array(&all_primes, &all_primes_cnt, &all_primes_cap, all_primes_cnt + 1);
            all_primes[all_primes_cnt++] = prime_candidate;

            /* if not in {2,3,7,13} store in series_primes */
            if (!(prime_candidate == 2 || prime_candidate == 3 ||
                  prime_candidate == 7 || prime_candidate == 13))
            {
                grow_uint64_array(&series_primes, &series_primes_cnt,
                                  &series_primes_cap, series_primes_cnt + 1);
                series_primes[series_primes_cnt++] = prime_candidate;
            }
        }
        ++prime_candidate;
    }
}

/*--- sequence function -------------------------------------*/
seq_t a(idx_t n)
{
    /* special entries */
    if (n == 1) { return (seq_t)5375; }
    if (n == 7) { return (seq_t)397535; }

    size_t series_idx;
    if (n < 7)
        series_idx = n - 1;   /* 2..6 -> 1..5 */
    else
        series_idx = n - 2;   /* >=8 -> series index continues */

    /* obtain the (series_idx)-th prime not dividing 546 */
    ensure_series_primes(series_idx);
    uint64_t p = series_primes[series_idx - 1];

    /* compute 546 * p^2 in seq_t */
    seq_t val = (seq_t)p;
    val = val * val;        /* p^2 */
    val = val * (seq_t)546; /* *546 */
    return val;
}
