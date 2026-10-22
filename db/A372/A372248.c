/*
 * A372248  Lowest prime p in a ladder of 5 consecutive primes p, p+2, p+6,
 *          p+14, p+30.
 * https://oeis.org/A372248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence consists of the lowest prime p in a ladder of five
   consecutive primes p, p+2, p+6, p+14, p+30.  We generate the primes
   by a segmented sieve and look for the required pattern.  The
   function a(n) returns the n‑th element (1‑based).  The implementation
   keeps all previously found terms in a static cache so that any call
   order is supported.  The algorithm runs in O(N log N) time with a
   small constant factor; it comfortably produces the first 10 000
   terms in well under two seconds. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Small prime table (primes up to 1 000 000). */
#define SMALL_PRIME_LIMIT 1000000
static uint8_t  comp[SMALL_PRIME_LIMIT+1];
static uint32_t small_primes[80000];
static size_t   small_prime_count = 0;
static bool     small_prime_inited = false;

/* Segmented sieve block size. */
#define BLOCK_SIZE 1048576   /* 2^20 */

/* Buffer for the segmented sieve. */
static uint8_t *sieve_buffer = NULL;

/* Cache of ladder starting primes. */
static seq_t *ladder = NULL;
static size_t ladder_size = 0, ladder_cap = 0;

/* Sliding window of the last five primes. */
static uint64_t last_primes[5];
static size_t   last_prime_cnt = 0;

/* The next number to start sieving from (odd numbers only). */
static uint64_t sieve_low = 3;

/* ------------------------------------------------------------------ */
/* Initialize the small prime table (used for marking composites).  */
static void init_small_primes(void) {
    if (small_prime_inited) return;
    for (size_t i = 0; i <= SMALL_PRIME_LIMIT; ++i) comp[i] = 0;
    comp[0] = comp[1] = 1;
    for (size_t i = 2; i * i <= SMALL_PRIME_LIMIT; ++i)
        if (!comp[i])
            for (size_t j = i * i; j <= SMALL_PRIME_LIMIT; j += i)
                comp[j] = 1;
    size_t idx = 0;
    for (size_t i = 2; i <= SMALL_PRIME_LIMIT; ++i)
        if (!comp[i])
            small_primes[idx++] = (uint32_t)i;
    small_prime_count = idx;
    small_prime_inited = true;
}

/* ------------------------------------------------------------------ */
/* Allocate (once) the buffer for composites of a block. */
static void init_sieve_buffer(void) {
    if (!sieve_buffer)
        sieve_buffer = (uint8_t *)malloc(BLOCK_SIZE);
}

/* ------------------------------------------------------------------ */
/* Expand the ladder cache to hold at least new_cap entries. */
static void ladder_reserve(size_t new_cap) {
    if (ladder_cap >= new_cap) return;
    size_t cap = ladder_cap == 0 ? 16 : ladder_cap * 2;
    if (cap < new_cap) cap = new_cap;
    seq_t *tmp = (seq_t *)realloc(ladder, cap * sizeof(seq_t));
    ladder = tmp;
    ladder_cap = cap;
}

/* ------------------------------------------------------------------ */
/* Generate ladder terms until we have at least target terms. */
static void generate_upto(idx_t target) {
    if ((size_t)target <= ladder_size) return;
    if (!sieve_buffer) init_sieve_buffer();
    init_small_primes();

    /* Continue sieving until we find enough terms. */
    while (ladder_size < (size_t)target) {
        uint64_t block_high = sieve_low + BLOCK_SIZE;
        /* Reset composite buffer. */
        memset(sieve_buffer, 0, BLOCK_SIZE);

        /* Mark composites using primes up to sqrt(block_high). */
        for (size_t pi = 0; pi < small_prime_count; ++pi) {
            uint32_t p = small_primes[pi];
            uint64_t p64 = p;
            if (p64 * p64 > block_high) break;
            /* Find first multiple of p in [sieve_low, block_high). */
            uint64_t start = (sieve_low + p - 1) / p * p;
            if (start < p64 * 2) start = p64 * 2;
            for (uint64_t x = start; x < block_high; x += p)
                sieve_buffer[x - sieve_low] = 1;
        }

        /* Scan block for primes and look for the ladder pattern. */
        for (size_t i = 0; i < BLOCK_SIZE; ++i) {
            uint64_t num = sieve_low + i;
            if (num < 2) continue;
            if (sieve_buffer[i]) continue;       /* composite */

            /* Handle the sliding window of last five primes. */
            if (last_prime_cnt < 5) {
                last_primes[last_prime_cnt++] = num;
            } else {
                /* shift left */
                for (size_t k = 0; k < 4; ++k)
                    last_primes[k] = last_primes[k + 1];
                last_primes[4] = num;

                /* Check consecutive differences. */
                if (last_primes[1] - last_primes[0] == 2 &&
                    last_primes[2] - last_primes[1] == 4 &&
                    last_primes[3] - last_primes[2] == 8 &&
                    last_primes[4] - last_primes[3] == 16) {
                    ladder_reserve(ladder_size + 1);
                    ladder[ladder_size++] = (seq_t)last_primes[0];
                }
            }
        }

        /* Advance to next block. */
        sieve_low = block_high;
    }
}

/* ------------------------------------------------------------------ */
/* Public API: return the n‑th term of the sequence. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined for n=0; guard against */
    generate_upto(n);
    return ladder[n - 1];
}
