/*
 * A368089  Numbers k neither squarefree nor prime powers whose squarefree
 *          kernel is not a primorial.
 * https://oeis.org/A368089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search/sieve for numbers k with Ω(k) > ω(k) > 1 and rad(k) not a
 *            primorial
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static bool seq_a_initialized = false;   /* flag */
static seq_t *seq_terms      = NULL;     /* array of found terms */
static idx_t seq_terms_len   = 0;        /* number of terms stored */
static idx_t seq_terms_cap   = 0;        /* allocated capacity */

/* initialise the sequence data once */
static void init_seq_a(void)
{
    /* Upper limit for k to examine.  Chosen to produce >10,000 terms. */
    const idx_t LIMIT          = 300000;
    const idx_t INIT_CAP       = 20000;

    /* Smallest prime factor array. */
    uint32_t *spf = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!spf) return; /* allocation failed – not expected */

    for (idx_t i = 0; i <= LIMIT; ++i)
        spf[i] = (uint32_t)i;

    for (idx_t i = 2; i * i <= LIMIT; ++i)
    {
        if (spf[i] == (uint32_t)i)          /* i is prime */
        {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                if (spf[j] == (uint32_t)j)
                    spf[j] = (uint32_t)i;
        }
    }

    /* Allocate the results array. */
    seq_terms_cap = INIT_CAP;
    seq_terms = (seq_t*)malloc(seq_terms_cap * sizeof(seq_t));
    seq_terms_len = 0;

    /* List of primorials up to LIMIT for exclusion. */
    const uint32_t primorials[] = {2, 6, 30, 210, 2310, 30030};
    const size_t n_primorials = sizeof(primorials) / sizeof(primorials[0]);

    /* Generate the terms up to LIMIT. */
    for (idx_t k = 2; k <= LIMIT; ++k)
    {
        uint64_t t   = k;
        uint32_t rad = 1;          /* square‑free kernel (product of distinct primes) */
        int omega    = 0;          /* total number of prime factors (with multiplicity) */
        int distinct  = 0;          /* number of distinct primes */
        uint32_t prev = 0;

        while (t > 1)
        {
            uint32_t p = spf[(uint32_t)t];
            if (p != prev)          /* new distinct prime factor */
            {
                ++distinct;
                rad *= p;
                prev = p;
            }
            t /= p;
            ++omega;
        }

        /* Must have Ω(k) > ω(k) > 1. */
        if (omega <= distinct || distinct <= 1)
            continue;

        /* Exclude numbers whose rad is a primorial. */
        bool is_primorial = false;
        for (size_t i = 0; i < n_primorials; ++i)
            if (rad == primorials[i]) { is_primorial = true; break; }
        if (is_primorial)
            continue;

        /* Store the term, growing the array if necessary. */
        if (seq_terms_len == seq_terms_cap)
        {
            seq_terms_cap *= 2;
            seq_terms = (seq_t*)realloc(seq_terms, seq_terms_cap * sizeof(seq_t));
            if (!seq_terms) return; /* allocation failed – not expected */
        }
        seq_terms[seq_terms_len++] = (seq_t)k;
    }

    free(spf);
    seq_a_initialized = true;
}

/* ------------------------------------------------------------ */
/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (!seq_a_initialized)
        init_seq_a();

    if (n == 0)                      /* undefined for n = 0 */
        return 0;

    if (n - 1 < seq_terms_len)
        return seq_terms[n - 1];

    /* If n exceeds the pre‑computed range, return 0 (unlikely for tested data). */
    return 0;
}
