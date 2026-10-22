/*
 * A377318  Numbers k such that prime(k), prime(k)+6, prime(k)+12, and
 *          prime(k)+18 are primes.
 * https://oeis.org/A377318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 279, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A377318: indices k such that prime(k), prime(k)+6,
 * prime(k)+12, and prime(k)+18 are all prime.
 *
 * The implementation builds a sieve of primes up to a hard‑coded
 * upper bound that contains more than 12 000 terms.  For any
 * requested n up to about 10 000 this bound is sufficient.
 *
 * On first call the sieve is computed and all qualifying indices
 * are stored in a static table.  Subsequent calls simply look
 * up the n‑th entry.  The function works with any seq_t (built‑in
 * or arbitrary‑precision) and any idx_t (uint64_t).  It compiles
 * under C11 and C++17.
 *
 * If n is 0 or beyond the computed length, 0 is returned.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_bits.h"

/* static state ------------------------------------------------------------- */
static seq_t *seq_table = NULL;    /* table of k values                */
static idx_t seq_len   = 0;        /* number of entries in seq_table   */
static size_t seq_cap  = 0;        /* allocated capacity of seq_table  */

/* --------------------------------------------------------------------------- */

/* --------------------------------------------------------------------------- */
/* Build the sieve and collect all k satisfying the quadruplet condition. */
static void precompute(void)
{
    if (seq_table != NULL) return;   /* already built */

    /* Upper bound for the sieve – enough for >12 000 qualifying primes.
     * This constant is chosen experimentally. */
    const unsigned int LIMIT = 200000000U;   /* 2·10⁸ */

    /* Number of odd numbers up to LIMIT, rounded up to whole 64‑bit words. */
    size_t odds   = LIMIT >> 1;
    size_t words  = (odds + 63) >> 6;

    /* Allocate and zero the bit sieve.  Cast to uint64_t* for C++ compatibility. */
    uint64_t *bits = (uint64_t *)calloc(words, sizeof(uint64_t));
    if (!bits) return;   /* allocation failure – leave seq_table NULL */

    /* Classic Eratosthenes sieve for odd numbers only. */
    for (unsigned int i = 3; (uint64_t)i * i <= LIMIT; i += 2) {
        unsigned int idx = (i >> 1) - 1;
        if ((bits[idx >> 6] >> (idx & 63)) & 1ULL) continue;   /* composite */
        unsigned int step = i << 1;   /* 2*i, step between odd multiples */
        for (uint64_t j = (uint64_t)i * i; j <= LIMIT; j += step) {
            unsigned int jidx = ((unsigned int)j >> 1) - 1;
            bits[jidx >> 6] |= 1ULL << (jidx & 63);
        }
    }

    /* Reserve an initial capacity – more than enough for 12 000 entries. */
    seq_cap = 12000;
    seq_table = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    if (!seq_table) {
        free(bits);
        return;   /* allocation failure */
    }
    seq_len = 0;

    idx_t prime_idx = 0;   /* current prime index (1‑based) */

    /* Process the prime 2 separately (it never satisfies the condition). */
    if (LIMIT >= 2) prime_idx++;   /* prime_idx now 1 */

    /* Scan all odd numbers in increasing order. */
    for (unsigned int n = 3; n <= LIMIT; n += 2) {
        if (!is_prime_bits(bits, n)) continue;   /* composite */
        prime_idx++;                            /* number of prime seen so far */

        /* Ensure the quadruplet stays within the sieve limit. */
        if ((uint64_t)n + 18U <= LIMIT) {
            if (is_prime_bits(bits, n + 6U) &&
                is_prime_bits(bits, n + 12U) &&
                is_prime_bits(bits, n + 18U)) {
                /* Store the index k (prime_idx) in the table. */
                if (seq_len == seq_cap) {          /* grow table if needed */
                    seq_cap <<= 1;                 /* double capacity */
                    seq_table = (seq_t *)realloc(seq_table,
                                                 seq_cap * sizeof(seq_t));
                    if (!seq_table) {              /* realloc failed */
                        free(bits);
                        seq_len = 0;
                        return;
                    }
                }
                seq_table[seq_len++] = (seq_t)prime_idx;
            }
        }
    }

    free(bits);   /* sieve no longer needed */
}

/* --------------------------------------------------------------------------- */
/* Public function: return the nth term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset 1 only, undefined before */

    if (seq_table == NULL) precompute();  /* build on first use */

    if (n > seq_len) return 0;            /* not enough terms precomputed */
    return seq_table[n - 1];
}
