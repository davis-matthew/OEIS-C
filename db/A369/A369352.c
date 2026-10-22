/*
 * A369352  Numbers k such that there exists a number p where p - k = sopfr(p)
 *          + sopfr(k), where sopfr(m) is the sum of the primes dividing m,
 *          with repetition.
 * https://oeis.org/A369352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 173, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sopfr_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A369352: Numbers k such that there exists p with
   p - k = sopfr(p) + sopfr(k), where sopfr(m) is the sum of the
   prime divisors of m with repetition. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sopfr_u.h"

/*--- global storage for the sequence -----------------------------------*/
static seq_t *terms = NULL;      /* array of discovered k's */
static size_t terms_cap = 0;     /* allocated capacity of terms      */
static size_t terms_len = 0;     /* number of terms actually stored */

/*--- helper: grow the terms array -------------------------------------*/
static void grow_terms(size_t new_cap)
{
    if (new_cap <= terms_cap) return;
    if (new_cap == 0) new_cap = 16;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    terms = tmp;
    terms_cap = new_cap;
}

/*--- static initialization routine -------------------------------------*/
static void init(void)
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    /* Limits: enough for n up to at least 10 000 */
    const idx_t K_LIMIT = 200000;          /* maximum k to scan      */
    const idx_t P_LIMIT = 2 * K_LIMIT;     /* sufficient for targets */

    /*--- build smallest prime factor array up to P_LIMIT ----*/
    uint32_t *spf = (uint32_t *)calloc((size_t)(P_LIMIT + 1), sizeof(uint32_t));
    if (!spf) abort();
    for (idx_t i = 2; i <= P_LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if ((uint64_t)i * i <= P_LIMIT) {
                for (idx_t j = i * i; j <= P_LIMIT; j += i)
                    if (spf[j] == 0)
                        spf[j] = (uint32_t)i;
            }
        }
    }

    /*--- mark which values of f(p)=p-sopfr(p) appear -----------------*/
    bool *f_seen = (bool *)calloc((size_t)(P_LIMIT + 1), sizeof(bool));
    if (!f_seen) abort();
    for (idx_t p = 2; p <= P_LIMIT; ++p) {
        seq_t sp = sopfr_u(p, spf);
        idx_t f = p - (idx_t)sp;
        if (f <= P_LIMIT) f_seen[f] = true;
    }

    /*--- generate the sequence in ascending order of k ---------------*/
    grow_terms(256);
    for (idx_t k = 1; k <= K_LIMIT; ++k) {
        seq_t sk = sopfr_u(k, spf);
        idx_t target = k + (idx_t)sk;
        if (target <= P_LIMIT && f_seen[target]) {
            if (terms_len == terms_cap) grow_terms(terms_cap * 2);
            terms[terms_len++] = (seq_t)k;
        }
    }

    /*--- clean up temporary arrays ------------------------------------*/
    free(spf);
    free(f_seen);
}

/*--- public function --------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* sequence starts at offset 1 */

    init(); /* ensure that terms are ready */

    if ((size_t)n > terms_len) return (seq_t)-1;  /* out of precomputed range */

    return terms[(size_t)(n - 1)];
}
