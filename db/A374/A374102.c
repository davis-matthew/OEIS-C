/*
 * A374102  Prime binary self (or Colombian) numbers: primes not expressible
 *          as the sum of an integer and its binary weight.
 * https://oeis.org/A374102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   No closed formula; compute Colombian primes by sieve (primes
 *            that are not equal to k + popcount(k))
 * Range:     u8 n <= 8, u16 n <= 1547, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Count 1‑bits of a 64‑bit integer.  Use built‑in if available. */
static inline uint32_t popcount_uint64(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return (uint32_t)__builtin_popcountll(x);
#else
    uint32_t c = 0;
    while (x) { x &= x - 1; ++c; }
    return c;
#endif
}

/* Lightweight capacity grower (different name from the provided helper). */
static void _ensure_capacity(void **ptr, size_t *cap, size_t elem_size, size_t min_cap)
{
    if (*cap < min_cap) {
        size_t new_cap = (*cap) ? (*cap << 1) : 8;
        while (new_cap < min_cap) new_cap <<= 1;
        void *tmp = realloc(*ptr, new_cap * elem_size);
        if (!tmp) abort();
        *ptr = tmp;
        *cap = new_cap;
    }
}

/* State that persists across calls to a(). */
struct seq_state {
    uint64_t *primes;          /* primes up to current limit */
    size_t   primes_len;
    size_t   primes_cap;

    unsigned char *rep;        /* rep[n] == 1 if n = k + popcount(k) */
    size_t   rep_cap;

    uint64_t *colombian;       /* Colombian primes */
    size_t   colombian_len;
    size_t   colombian_cap;

    uint64_t limit;            /* current sieving limit */
    bool initialized;
};

static struct seq_state state = {0};

/* Build sieve and representation table up to 'new_limit'. */
static void build_up_to(uint64_t new_limit)
{
    /* Allocate primes array. */
    size_t approx = new_limit/2 + 10;  /* rough room for primes */
    _ensure_capacity((void **)&state.primes, &state.primes_cap,
                     sizeof(uint64_t), approx);
    state.primes_len = 0;

    /* Simple Eratosthenes sieve on a byte array. */
    unsigned char *is_comp = (unsigned char *)calloc((size_t)(new_limit + 1), 1);
    if (!is_comp) abort();

    for (uint64_t i = 2; i <= new_limit; ++i) {
        if (!is_comp[(size_t)i]) {
            state.primes[state.primes_len++] = i;
            if (i * i <= new_limit) {
                for (uint64_t j = i * i; j <= new_limit; j += i)
                    is_comp[(size_t)j] = 1;
            }
        }
    }

    /* Allocate and zero representation table. */
    _ensure_capacity((void **)&state.rep, &state.rep_cap,
                     sizeof(unsigned char), (size_t)(new_limit + 1));
    memset(state.rep, 0, (size_t)(new_limit + 1));

    /* Mark all numbers that can be written as k + popcount(k). */
    for (uint64_t k = 0; k <= new_limit; ++k) {
        uint64_t s = k + popcount_uint64(k);
        if (s <= new_limit) state.rep[(size_t)s] = 1;
    }

    /* Collect Colombian primes. */
    _ensure_capacity((void **)&state.colombian, &state.colombian_cap,
                     sizeof(uint64_t), state.primes_len);
    state.colombian_len = 0;
    for (size_t i = 0; i < state.primes_len; ++i) {
        uint64_t p = state.primes[i];
        if (!state.rep[(size_t)p]) state.colombian[state.colombian_len++] = p;
    }

    state.limit = new_limit;
    state.initialized = true;
    free(is_comp);
}

/* Public function required by the harness. */
seq_t a(idx_t n)
{
    /* Ensure at least a few primes to start. */
    if (!state.initialized) {
        build_up_to(200000);
    }

    /* Keep enlarging the sieve until we have enough Colombian primes. */
    while (state.colombian_len < (size_t)n) {
        uint64_t new_limit = state.limit * 2 + 1;
        build_up_to(new_limit);
    }

    return (seq_t)state.colombian[n - 1];
}
