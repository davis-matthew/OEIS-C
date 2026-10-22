/*
 * A371022  Numbers k with the property that there is a finite set W of groups
 *          of order divisible by k such that if k divides the order of a
 *          group G, then G has a subgroup isomorphic to a group in W.
 * https://oeis.org/A371022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A371022 – Cauchy numbers
    a(n) = the nth integer k such that
           k is a prime power,
           or k = 6,
           or k = 2·p^a where p is a Fermat prime > 3.           */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t *seq_vals   = NULL;    /* sorted list of found terms */
static size_t seq_size   = 0;       /* number of terms stored      */
static idx_t   seq_limit = 0;       /* numeric limit used           */

/* known Fermat primes greater than 3 */
static const uint32_t fermat_primes[] = {5, 17, 257, 65537};
static const size_t   fermat_cnt      = sizeof(fermat_primes)/sizeof(fermat_primes[0]);

/* Build the sequence up to numeric limit `limit`. */
static void build_sequence(idx_t limit)
{
    /* free previous data */
    if (seq_vals) { free(seq_vals); seq_vals = NULL; }
    seq_size = 0;
    seq_limit = limit;

    /* flags to mark numbers that are in the sequence */
    uint8_t *flag = (uint8_t*)calloc((size_t)(limit + 1), 1);
    if (!flag) return; /* allocation failed */

    /* sieve for primes up to `limit` */
    uint8_t *is_prime = (uint8_t*)calloc((size_t)(limit + 1), 1);
    if (!is_prime) { free(flag); return; }
    if (limit >= 2) is_prime[2] = 1;
    for (idx_t i = 3; i <= limit; i += 2) is_prime[i] = 1;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!is_prime[i]) continue;
        for (idx_t j = i * i; j <= limit; j += i) is_prime[j] = 0;
    }

    /* mark prime powers and special 2·p^a values */
    for (idx_t p = 2; p <= limit; ++p) {
        if (!is_prime[p]) continue;          /* p is a prime */

        /* prime powers p^a */
        for (idx_t x = p; x <= limit; x *= p) {
            flag[x] = 1;
            if (x > limit / p) break;
        }

        /* check if p is a Fermat prime > 3 */
        bool is_fermat = false;
        for (size_t k = 0; k < fermat_cnt; ++k) {
            if (fermat_primes[k] == p) { is_fermat = true; break; }
        }
        if (!is_fermat) continue;

        /* numbers of the form 2·p^a */
        for (idx_t x = p; x <= limit; ) {
            idx_t val = 2 * x;
            if (val > limit) break;
            flag[val] = 1;
            if (x > limit / p) break;
            x *= p;
        }
    }

    /* always include 6 */
    if (limit >= 6) flag[6] = 1;

    /* count numbers for allocation */
    size_t count = 0;
    for (idx_t i = 2; i <= limit; ++i) if (flag[i]) ++count;

    if (count > 0) {
        seq_vals = (seq_t*)malloc(count * sizeof(seq_t));
        if (seq_vals) {
            size_t idx = 0;
            for (idx_t i = 2; i <= limit; ++i)
                if (flag[i]) seq_vals[idx++] = (seq_t)i;
            seq_size = count;
        }
    }

    free(flag);
    free(is_prime);
}

/* Ensure that at least `need` terms are computed.  If more are required
   rebuild with a larger numeric limit.  This function is not reentrant. */
static void ensure_sequence(idx_t need)
{
    if (seq_size >= (size_t)need) return;
    idx_t limit = (seq_limit > 0) ? seq_limit * 2 : 256;
    while (seq_size < (size_t)need) {
        build_sequence(limit);
        if (seq_size >= (size_t)need) break;
        limit <<= 1;  /* double the limit for next attempt */
    }
}

/* ------------------------------------------------------------------ */
/*  Public function requested by the harness.                         */
/*  Returns the nth term (1‑based) of sequence A371022.                */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;      /* undefined for n < 1 */
    ensure_sequence(n);
    if ((size_t)n <= seq_size) return seq_vals[(size_t)n - 1];
    return (seq_t)0;                 /* should not happen */
}
