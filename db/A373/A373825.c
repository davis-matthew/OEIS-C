/*
 * A373825  Position of first appearance of n in the run-lengths (differing by
 *          0) of the run-lengths (differing by 2) of the odd primes.
 * https://oeis.org/A373825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A373825: a(n) = first position of n in the
 * run-lengths (differing by 0) of the run-lengths (differing by 2)
 * of the odd primes. The function scans odd primes in order,
 * records runs of consecutive twin primes, then records runs of
 * equal run-lengths, keeping the first occurrence position of
 * each possible value. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Parameters: chosen to cover n up to about 10 000 comfortably. */
#define PRIME_LIMIT   30000000ULL       /* upper bound for the primes sieve   */
#define MAX_TERM_VAL  200000              /* largest value for which we store
                                            * the first occurrence position   */

/* --------------------------------------------------------------------- */
/* Bit‑set helpers for the sieve (odd numbers only). */
static inline void bit_set(uint8_t *sieve, uint32_t bit) {
    sieve[bit >> 3] |= (1U << (bit & 7));
}
static inline bool bit_get(const uint8_t *sieve, uint32_t bit) {
    return (sieve[bit >> 3] >> (bit & 7)) & 1U;
}

/* --------------------------------------------------------------------- */
/* Global data, built on first call to a(). */
static bool data_ready = false;
static uint32_t *primes = NULL;
static size_t   prime_cnt = 0;
static idx_t   *first_pos = NULL;   /* first_pos[v] = first index in Q where v occurs */

/* --------------------------------------------------------------------- */
/* Build the list of odd primes up to PRIME_LIMIT. */
static void build_primes(void) {
    uint64_t limit = PRIME_LIMIT;
    /* bit‑set size for odd numbers up to limit */
    size_t bits_needed = (limit >> 1) + 1;   /* number of odd numbers */
    size_t byte_size  = (bits_needed >> 3) + 1;
    uint8_t *sieve = (uint8_t*)calloc(byte_size, 1);
    if (!sieve) exit(1);

    /* Eratosthenes' sieve (odd numbers only). */
    uint32_t i, j;
    for (i = 3; (uint64_t)i * i <= limit; i += 2) {
        uint32_t bit_i = i >> 1;
        if (!bit_get(sieve, bit_i)) {
            uint64_t step = (uint64_t)i << 1;
            for (uint64_t k = (uint64_t)i * i; k <= limit; k += step) {
                bit_set(sieve, (uint32_t)(k >> 1));
            }
        }
    }

    /* Count primes and allocate array. */
    size_t cap = limit / 10;          /* heuristic */
    primes = (uint32_t*)malloc(cap * sizeof(uint32_t));
    if (!primes) exit(1);

    /* 3 is the first odd prime. */
    prime_cnt = 0;
    for (i = 3; i <= limit; i += 2) {
        if (!bit_get(sieve, i >> 1)) {
            if (prime_cnt == cap) {
                cap <<= 1;
                uint32_t *tmp = (uint32_t*)realloc(primes, cap * sizeof(uint32_t));
                if (!tmp) exit(1);
                primes = tmp;
            }
            primes[prime_cnt++] = i;
        }
    }

    free(sieve);
}

/* --------------------------------------------------------------------- */
/* Build the first‑position table for values up to MAX_TERM_VAL. */
static void build_first_pos(void) {
    first_pos = (idx_t*)calloc((size_t)MAX_TERM_VAL + 1, sizeof(idx_t));
    if (!first_pos) exit(1);

    /* Run variables for generating R and Q. */
    const uint32_t *p = primes;
    /* R: run‑lengths of consecutive twin primes (diff==2). */
    uint32_t current_twin_len = 1;  /* start with first prime */
    uint32_t prev_prime = p[0];

    /* Q: run‑lengths of equal R values. */
    int prev_R = -1;
    uint32_t run_R = 0;
    idx_t pos_Q = 1;                 /* 1‑based position in Q */

    /* Helper macros to record R and Q values. */
    #define RECORD_R(val)                \
        do {                            \
            if (prev_R == -1) {         \
                prev_R = (int)(val);    \
                run_R  = 1;             \
            } else if ((int)(val) == prev_R) { \
                run_R++;                \
            } else {                    \
                /* finalize previous Q value */ \
                if (run_R <= MAX_TERM_VAL && first_pos[run_R] == 0) \
                    first_pos[run_R] = pos_Q; \
                pos_Q++; \
                prev_R = (int)(val);    \
                run_R = 1;              \
            }                           \
        } while (0)

    #define FINALIZE_Q()                  \
        do {                              \
            if (run_R <= MAX_TERM_VAL && first_pos[run_R] == 0) \
                first_pos[run_R] = pos_Q; \
            pos_Q++;                       \
        } while (0)

    /* Process all primes. */
    for (size_t i = 1; i < prime_cnt; ++i) {
        uint32_t cur = p[i];
        uint32_t diff = cur - prev_prime;
        if (diff == 2) {
            current_twin_len++;
        } else {
            /* R value finished. */
            RECORD_R(current_twin_len);
            current_twin_len = 1; /* start new run at cur */
        }
        prev_prime = cur;
    }
    /* Final R value for the last run. */
    RECORD_R(current_twin_len);
    /* Final Q value after processing all R. */
    FINALIZE_Q();

    #undef RECORD_R
    #undef FINALIZE_Q

    data_ready = true;
}

/* --------------------------------------------------------------------- */
/* The required function. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;           /* undefined position */
    if (!data_ready) {
        build_primes();
        build_first_pos();
    }
    if (n <= (idx_t)MAX_TERM_VAL && first_pos[(size_t)n] != 0)
        return (seq_t)first_pos[(size_t)n];
    return (seq_t)0;  /* value not yet seen within the computed range */
}
