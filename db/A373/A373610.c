/*
 * A373610  Positive integers that cannot be written as a sum of a practical
 *          number and an octagonal number.
 * https://oeis.org/A373610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 293, u32 n <= 414, u64 n <= 414,
 *            u128 n <= 414, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A373610: Positive integers that cannot be written as a sum of
   a practical number and an octagonal number.
   The function generates the sequence by brute‑force search,
   storing the found terms in a static array.  It precomputes all
   practical numbers up to a fixed limit using the standard
   Erdos‑Nicolas characterization.  */
/* Sequence formula: brute force search */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* maximum number to pre‑compute practical numbers for */
#define LIMIT 5000000U

/* forward declarations of helper arrays */
static uint32_t *spf   = NULL;   /* smallest prime factor for 0 .. LIMIT */
static bool    *is_practical = NULL;

/* cache for sequence values */
static seq_t *seq_vals = NULL;
static size_t seq_len  = 0;   /* number of values stored */
static size_t seq_cap  = 0;   /* allocated capacity of seq_vals */
static uint32_t last_checked = 0;  /* last integer examined (>= last cached value) */

/* ------------------------------------------------------------------ */
/* Build sieve for smallest prime factor up to LIMIT */
static void build_spf(void)
{
    spf = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!spf) exit(EXIT_FAILURE);
    for (uint32_t i = 0; i <= LIMIT; ++i) spf[i] = 0;

    uint32_t *pr = (uint32_t*)malloc((LIMIT / 10) * sizeof(uint32_t));
    if (!pr) exit(EXIT_FAILURE);
    size_t pcnt = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            pr[pcnt++] = i;
        }
        for (size_t j = 0; j < pcnt && pr[j] <= spf[i]
             && (uint64_t)i * pr[j] <= LIMIT; ++j) {
            spf[i * pr[j]] = pr[j];
        }
    }
    free(pr);
}

/* ------------------------------------------------------------------ */
/* Practical number test using Erdos–Nicolas criterion.
   Input:   n <= LIMIT
   Output:  true  if n is practical, false otherwise
*/
static bool check_practical(uint32_t n)
{
    if (n == 1) return true;

    uint32_t num = n;
    bool first = true;
    uint64_t sum_sigma = 1;        /* product of sigma(p^e) for processed primes */

    while (num > 1) {
        uint32_t p = spf[num];
        uint32_t e = 0;
        while (num % p == 0) {
            num /= p;
            ++e;
        }
        if (first) {
            if (p != 2) return false;
            first = false;
        } else {
            if (p > sum_sigma + 1) return false;
        }
        /* compute sigma(p^e) = 1 + p + ... + p^e */
        uint64_t sigma = 1;
        uint64_t pk = 1;
        for (uint32_t i = 0; i < e; ++i) {
            pk *= p;
            sigma += pk;
        }
        sum_sigma *= sigma;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Precompute practical numbers up to LIMIT */
static void precompute_practical(void)
{
    is_practical = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_practical) exit(EXIT_FAILURE);
    is_practical[0] = false;
    for (uint32_t i = 1; i <= LIMIT; ++i) {
        is_practical[i] = check_practical(i);
    }
}

/* ------------------------------------------------------------------ */
/* Ensure capacity for seq_vals array. */
static void ensure_seq_cap(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_vals = (seq_t*)realloc(seq_vals, new_cap * sizeof(seq_t));
    if (!seq_vals) exit(EXIT_FAILURE);
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Main function for the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for offset 0 */

    static bool initialized = false;
    if (!initialized) {
        build_spf();
        precompute_practical();
        initialized = true;
    }

    /* Generate terms until we have at least n. */
    while (seq_len < n) {
        ++last_checked;
        bool representable = false;

        /* A number equal to a practical number is representable
           via adding the octagonal 0. */
        if (last_checked <= LIMIT && is_practical[(uint32_t)last_checked]) {
            representable = true;
        } else {
            /* Try all octagonal numbers o >= 1. */
            for (uint32_t k = 1; ; ++k) {
                uint32_t oct = k * (3U * k - 2U);
                if (oct > last_checked) break;
                uint32_t p = last_checked - oct;
                if (p <= LIMIT && is_practical[p]) {
                    representable = true;
                    break;
                }
            }
        }

        if (!representable) {
            ensure_seq_cap(seq_len + 1);
            seq_vals[seq_len++] = (seq_t)last_checked;
        }
    }

    return seq_vals[n - 1];
}
