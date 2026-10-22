/*
 * A337136  a(1) = 1, a(2) = 2; for n > 1, a(n) = smallest positive number
 *          which has not yet appeared which has a common factor with a(n-2) +
 *          a(n-1).
 * https://oeis.org/A337136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A337136:  
   a(1) = 1, a(2) = 2; for n>2,
   a(n) = smallest positive integer not yet appeared which shares a
   common factor with a(n-2)+a(n-1).  The sequence is a permutation of
   the natural numbers.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static seq_t *seq = NULL;           /* computed sequence values           */
static size_t seq_cap = 0;          /* capacity of seq array              */
static size_t seq_len = 0;          /* number of terms actually stored    */

static uint8_t *used = NULL;        /* bitmap: used[i]==1 iff value i used*/
static size_t used_cap = 0;         /* capacity of used array (bytes)     */

/* enlarge the used bitmap so that indices up to 'needed' (inclusive) are addressable */
static void ensure_used_capacity(uint64_t needed)
{
    if (needed < used_cap)
        return;
    size_t new_cap = used_cap ? used_cap << 1 : 1;
    while (new_cap <= needed)
        new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(used, new_cap * sizeof(uint8_t));
    if (!tmp)
        abort();                      /* out of memory */
    used = tmp;
    memset(used + used_cap, 0, (new_cap - used_cap) * sizeof(uint8_t));
    used_cap = new_cap;
}

/* enlarge the seq array so that at least 'needed' elements are addressable */
static void ensure_seq_capacity(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 1;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                      /* out of memory */
    seq = tmp;
    seq_cap = new_cap;
}

/* factorise n into its distinct prime factors */
static void factor_unique(uint64_t n, uint64_t *factors, size_t *cnt)
{
    *cnt = 0;
    for (uint64_t p = 2; (__uint128_t)p * p <= n; ++p) {
        if (n % p == 0) {
            factors[(*cnt)++] = p;
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1)
        factors[(*cnt)++] = n;
}

/* return the smallest unused multiple of prime ≥ prime */
static uint64_t next_unused_multiple(uint64_t prime)
{
    uint64_t cand = prime;
    while (true) {
        ensure_used_capacity(cand);
        if (!used[cand])
            return cand;
        cand += prime;
    }
}

/* public function ---------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined, but return dummy */

    /* initialise first two terms on demand */
    if (seq_len == 0) {
        ensure_seq_capacity(2);
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)2;
        seq_len = 2;

        ensure_used_capacity(2);
        used[1] = 1;
        used[2] = 1;
    }

    /* extend sequence up to index n (1‑based) */
    while (seq_len < (size_t)n) {
        seq_t a_nm2 = seq[seq_len - 2];
        seq_t a_nm1 = seq[seq_len - 1];
        uint64_t sum = (uint64_t)a_nm2 + (uint64_t)a_nm1;

        /* factorise sum into distinct prime factors */
        uint64_t primes[64];
        size_t prime_cnt;
        factor_unique(sum, primes, &prime_cnt);

        /* find the smallest unused multiple among these primes */
        uint64_t best = UINT64_MAX;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t cand = next_unused_multiple(primes[i]);
            if (cand < best)
                best = cand;
        }

        seq_t val = (seq_t)best;
        ensure_seq_capacity(seq_len + 1);
        seq[seq_len++] = val;

        /* mark this value as used */
        ensure_used_capacity((uint64_t)val);
        used[(uint64_t)val] = 1;
    }

    return seq[n - 1];
}
