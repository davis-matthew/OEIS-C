/*
 * A332514  Numbers k such that phi(k) == 6 (mod 12), where phi is the Euler
 *          totient function (A000010).
 * https://oeis.org/A332514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 2574, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A332514: numbers k such that φ(k) ≡ 6 (mod 12).
   This implementation uses a simple sieve to compute Euler's totient
   values up to a fixed bound (2 000 000); it then collects all k whose
   totient satisfies the congruence. The resulting list is stored
   in a static array so that subsequent calls are O(1).  */
   
#include <stdint.h>
#include <stdlib.h>

static const idx_t INITIAL_LIMIT = 2000000ULL;
static uint64_t *phi_arr  = NULL;   /* computed φ values      */
static seq_t  *terms     = NULL;   /* list of A332514 terms  */
static size_t  terms_len = 0;       /* current number of terms*/
static size_t  terms_cap = 0;       /* allocated capacity     */
static idx_t   φ_limit   = 0;       /* up to which φ was computed */
static int     initialized = 0;     /* flag whether tables ready */

/* Grow the terms array to accommodate at least `needed` elements. */
static void _grow_terms(size_t needed)
{
    if (terms_cap >= needed) return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_arr) abort();      /* out of memory */
    terms     = new_arr;
    terms_cap = new_cap;
}

/* Build φ values up to `limit` using a standard linear sieve of
   Euler's totient function. After the call, φ_arr[i] = φ(i) for
   0 ≤ i ≤ limit. */
static void _build_phi(idx_t limit)
{
    if (phi_arr) free(phi_arr);
    phi_arr = (uint64_t *)malloc((limit + 1) * sizeof(uint64_t));
    if (!phi_arr) abort();           /* out of memory */

    for (idx_t i = 0; i <= limit; ++i)
        phi_arr[i] = i;

    for (idx_t i = 2; i <= limit; ++i) {
        if (phi_arr[i] == (uint64_t)i) {          /* i is prime   */
            for (idx_t j = i; j <= limit; j += i)
                phi_arr[j] -= phi_arr[j] / i;     /* φ(j) *= (1‑1/p) */
        }
    }
    φ_limit = limit;
}

/* Populate the list of terms from `1` up to `limit`.  Only values
   that fit into `seq_t` are stored; the type naturally truncates
   larger numbers which will never be requested in the small‑type
   builds. */
static void _build_terms(idx_t limit)
{
    if (!phi_arr) _build_phi(limit);
    else if (limit > φ_limit) _build_phi(limit);   /* recompute if needed */

    _grow_terms(limit / 10 + 1000); /* rough initial capacity */

    for (idx_t k = 2; k <= limit; ++k) {
        if ((phi_arr[k] % 12ULL) == 6ULL) {
            _grow_terms(terms_len + 1);
            terms[terms_len++] = (seq_t)k;
        }
    }
}

/* Ensure the terms array contains at least `n` elements.  If it
   currently has fewer, the sieve limit is doubled until enough
   terms are found. */
static void _ensure_terms(size_t n)
{
    if (n <= terms_len) return;

    idx_t limit = INITIAL_LIMIT;
    while (1) {
        _build_terms(limit);
        if (terms_len >= n) break;
        limit <<= 1;                      /* double the limit */
        /* free the old φ array to save memory */
        free(phi_arr);
        phi_arr = NULL;
        terms_len = 0;
    }
}

/* Public entry point.  All calls may come in arbitrary order; the
   function performs a one‑time initialization to compute enough
   terms to serve the requested index.  */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    if (!initialized) {
        _ensure_terms(1000);      /* build enough for typical first queries */
        initialized = 1;
    } else {
        _ensure_terms(n);         /* ensure we can answer this request */
    }
    return terms[n - 1];
}
