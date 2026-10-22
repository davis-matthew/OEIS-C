/*
 * A377028  a(n) = number of integers k <= n such that k is a term in A055932.
 * https://oeis.org/A377028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ k ≤ n : rad(k) is a primorial }|
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes this count using a linear sieve for
 * smallest prime factors, then builds the square‑free kernel of
 * each integer and tests whether it is a primorial (product of
 * the first m primes, 1 is treated as a primorial).  
 * Results are memoised in a static cumulative array so that
 * repeated calls (in any order) are O(1) after the first
 * expansion to the maximum requested n. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Forward declaration for the static memory holder */
static struct {
    idx_t cap;          /* size of allocated arrays (indexable up to cap-1) */
    seq_t *pref;        /* cumulative counts a(i) for 0 ≤ i < cap */
} _A377028_state = {0, NULL};

/* Ensure that the cumulative array is grown to cover at least n.
 * When growing, a full recomputation up to the new capacity is
 * performed.  The capacity is always at least twice the previous
 * size to keep the overall cost amortised. */
static void _ensure_idx(idx_t n)
{
    if (n < _A377028_state.cap) return;           /* already covered */

    /* Determine new capacity (double previous, or at least n+1) */
    idx_t new_cap = n + 1;
    if (_A377028_state.cap > 0 && new_cap <= _A377028_state.cap * 2)
        new_cap = _A377028_state.cap * 2;
    if (new_cap <= 1) new_cap = 2;                 /* for tiny n */

    /* Allocate temporary arrays */
    uint32_t *spf      = (uint32_t*)malloc(new_cap * sizeof(uint32_t));
    uint64_t *rad      = (uint64_t*)malloc(new_cap * sizeof(uint64_t));
    bool     *isPrim   = (bool*)calloc(new_cap, sizeof(bool));
    uint32_t *primes   = (uint32_t*)malloc(new_cap * sizeof(uint32_t));
    if (!spf || !rad || !isPrim || !primes) {
        /* Memory allocation failed – abort gracefully */
        exit(EXIT_FAILURE);
    }

    /* Linear sieve to compute smallest prime factor and list of primes */
    for (idx_t i = 2; i < new_cap; ++i) spf[i] = 0;
    size_t prime_cnt = 0;
    for (idx_t i = 2; i < new_cap; ++i) {
        if (spf[i] == 0) {            /* i is prime */
            spf[i] = (uint32_t)i;
            primes[prime_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < prime_cnt &&
             primes[j] <= spf[i] &&
             i * (idx_t)primes[j] < new_cap;
             ++j)
            spf[i * (idx_t)primes[j]] = primes[j];
    }

    /* Compute squarefree kernel (rad) for each integer */
    rad[1] = 1;
    for (idx_t i = 2; i < new_cap; ++i) {
        uint32_t p  = spf[i];
        idx_t    q  = i / p;
        if (q % p == 0)
            rad[i] = rad[q];                 /* p already taken into account */
        else
            rad[i] = rad[q] * (uint64_t)p;
    }

    /* Determine all primorial numbers up to new_cap-1 */
    isPrim[1] = true;                         /* 1 is considered a primorial */
    uint64_t prod = 1;
    for (size_t j = 0; j < prime_cnt; ++j) {
        prod *= primes[j];
        if (prod >= (uint64_t)new_cap) break;
        isPrim[(idx_t)prod] = true;
    }

    /* Build cumulative counts */
    seq_t *new_pref = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_pref) exit(EXIT_FAILURE);
    new_pref[0] = (seq_t)0;
    for (idx_t i = 1; i < new_cap; ++i) {
        seq_t add = isPrim[(idx_t)rad[i]] ? (seq_t)1 : (seq_t)0;
        new_pref[i] = new_pref[i - 1] + add;
    }

    /* Replace old state with the new one */
    free(_A377028_state.pref);
    _A377028_state.pref = new_pref;
    _A377028_state.cap  = new_cap;

    /* Free temporary arrays */
    free(spf);
    free(rad);
    free(isPrim);
    free(primes);
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for 0, but safe guard */
    _ensure_idx(n);
    return _A377028_state.pref[n];
}
