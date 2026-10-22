/*
 * A394860  Numbers that are neither squarefree nor squareful, with exponents
 *          of prime power factors that are distinct.
 * https://oeis.org/A394860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 8159, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: search for numbers that are neither squarefree nor
 * squareful, with all prime‑power exponents distinct. The function
 * keeps a cache of terms and extends a small‑prime‑factor sieve as
 * needed. */

/* header(s) that are always available */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --- global state ----------------------------------------------------- */
static seq_t *cache = NULL;          /* cached sequence values */
static size_t cache_cap = 0;
static size_t cache_len = 0;         /* number of cached terms */

static idx_t current_candidate = 2;  /* next integer to test */

static idx_t sieve_limit = 5000000ULL;   /* current sieve limit */
static uint32_t *spf = NULL;             /* smallest prime factor array */

/* --- sieve initialisation -------------------------------------------- */
static void init_sieve(void)
{
    spf = (uint32_t*)calloc(sieve_limit + 1, sizeof(uint32_t));
    if (!spf) exit(1);

    for (idx_t i = 2; i <= sieve_limit; ++i) {
        if (spf[i] == 0) {              /* i is prime */
            spf[i] = (uint32_t)i;
            if (i <= sieve_limit / i) {
                for (idx_t j = i * i; j <= sieve_limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }
}

/* --- extend the sieve when needed ----------------------------------- */
static void extend_sieve(void)
{
    idx_t new_limit = sieve_limit * 2 + 1;   /* double and add one */
    free(spf);
    spf = NULL;
    sieve_limit = new_limit;
    init_sieve();
}

/* --- test if a 64‑bit value satisfies the property ------------------ */
static bool good_uint64(uint64_t val)
{
    if (val <= 3) return false;          /* smallest term is 12 */

    bool seen[65] = { false };           /* exponents 1..64 */
    bool has1 = false, has_ge2 = false;

    while (val > 1) {
        uint32_t p = spf[(idx_t)val];
        int exp = 0;
        while (val % p == 0) {
            val /= p;
            ++exp;
        }

        if (exp == 1)
            has1 = true;
        else
            has_ge2 = true;

        if (exp <= 64) {
            if (seen[exp]) return false;   /* duplicate exponent */
            seen[exp] = true;
        }
    }

    return has1 && has_ge2;
}

/* --- ensure cache contains at least n terms -------------------------- */
static void ensure_terms(idx_t n)
{
    if (!spf) init_sieve();               /* initialise sieve once */

    while (cache_len < (size_t)n) {
        if (current_candidate > sieve_limit) {
            extend_sieve();                /* enlarge sieve if necessary */
            continue;
        }

        if (good_uint64((uint64_t)current_candidate)) {
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 128;
                cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!cache) exit(1);
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)current_candidate;
        }
        ++current_candidate;
    }
}

/* --- public function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, return 0 */
    ensure_terms(n);
    return cache[n - 1];
}
