/*
 * A333216  Lengths of maximal subsequences without adjacent equal terms in
 *          the sequence of prime gaps.
 * https://oeis.org/A333216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hear
 * Formula:   a(n) = length of the nth maximal subsequence of consecutive *
 *            prime gaps with all adjacent gaps distinct. The sequence is
 *            obtained * by generating the prime gaps G = (p_{k+1}-p_k) and
 *            splitting G into * maximal blocks where G_i != G_{i+1}. Each
 *            block's length is a term * of the sequence
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function depends only on the standard library and the helper
 * functions supplied by the build system.  It works for any seq_t
 * (basic integer or arbitrary‑precision type). */
#include <stdlib.h>
#include <string.h>   /* for memcpy */

/* Holds the sequence values; lazily initialised on the first call to a(). */
static seq_t *seq = NULL;
static size_t seq_len = 0;     /* number of elements stored in seq   */
static size_t seq_cap = 0;     /* allocated capacity in seq          */

/* Grow seq to at least new_cap entries.  A new buffer is allocated,
 * existing elements are copied via assignment (which is safe for
 * arbitrary‑precision types), and the old buffer freed. */
static void grow_seq(size_t new_cap)
{
    seq_t *new_buf = (seq_t *)calloc(new_cap, sizeof(seq_t));
    if (!new_buf) return;  /* allocation failure – leave seq unchanged */

    for (size_t i = 0; i < seq_len; ++i)
        new_buf[i] = seq[i];

    free(seq);
    seq = new_buf;
    seq_cap = new_cap;
}

/* Append a value to seq, resizing as necessary. */
static void append_seq(seq_t v)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        grow_seq(new_cap);
    }
    seq[seq_len++] = v;
}

/* Build the sequence up to a predefined prime limit.  The limit
 * guarantees more than 10 000 terms for the intended tests. */
static void init_sequence(void)
{
    /* Prime sieve limit: 4 million is more than enough for ~10 000 terms. */
    const idx_t LIMIT = 4000000;

    /* Allocate and initialise the prime mask. */
    unsigned char *is_prime = (unsigned char *)calloc(LIMIT + 1, 1);
    if (!is_prime) return;  /* allocation failure – no sequence will be built */
    for (idx_t i = 2; i <= LIMIT; ++i) is_prime[i] = 1;

    /* Eratosthenes sieve. */
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* Collect the primes into a dynamic array. */
    size_t primes_cap = 256;
    size_t primes_len = 0;
    idx_t *primes = (idx_t *)calloc(primes_cap, sizeof(idx_t));
    if (!primes) { free(is_prime); return; }

    for (idx_t i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            if (primes_len == primes_cap) {
                size_t new_cap = primes_cap * 2;
                idx_t *tmp = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
                if (!tmp) { free(is_prime); free(primes); return; }
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_len++] = i;
        }
    }
    free(is_prime);

    /* Compute the lengths of maximal unequal‑gap blocks. */
    if (primes_len >= 2) {
        idx_t prev_gap = primes[1] - primes[0];
        seq_t cur_len  = 1;  /* first gap starts the first block */
        for (size_t i = 2; i < primes_len; ++i) {
            idx_t gap = primes[i] - primes[i-1];
            if (gap == prev_gap) {      /* block ends */
                append_seq(cur_len);
                cur_len = 1;
            } else {                    /* block continues */
                ++cur_len;
            }
            prev_gap = gap;
        }
        append_seq(cur_len);  /* last block */
    }

    free(primes);
}

/* Public entry point -------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Lazily build the whole sequence on the first call. */
    static int initialized = 0;
    if (!initialized) {
        init_sequence();
        initialized = 1;
    }

    /* Return the requested term if it exists; otherwise return 0. */
    return (n > 0 && (size_t)n <= seq_len) ? seq[n-1] : (seq_t)0;
}
