/*
 * A339451  Gray-code-like sequence in which, at each step, the least
 *          significant bit that has never been toggled from the previous
 *          value, is toggled.
 * https://oeis.org/A339451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0 * For n > 0, a(n) = a(n-1) XOR (1 << k), * where k is
 *            the count of how many times the value * a(n-1) has already been
 *            the left state before this step. * Each time a state is left we
 *            toggle the next least * significant unused bit. * * This
 *            implements the Gray-code-like rule: * “At each st...
 * Range:     u8 n <= 510, u16 n <= 9457, u32 n <= 9457, u64 n <= 9457,
 *            u128 n <= 9457, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Entry for simple open-addressing hash table.
 * Keys are truncated to uint64_t; this suffices because
 * the sequence values for n ≤ 10000 stay below 2^64.
 */
typedef struct {
    uint64_t key;
    unsigned cnt;     /* next bit to toggle from this key */
    unsigned used;    /* 0 = empty, 1 = occupied */
} Entry;

/* Multiplicative hash for 64‑bit keys */
static inline size_t hash64(uint64_t x) {
    /* Knuth's multiplicative method with a large odd constant */
    return (size_t)((x * 11400714785074694791ULL) >> 32);
}

/* Main sequence generator */
seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0;
    }

    /* Allocate a hash table large enough for at most 2*n+2 entries */
    size_t cap = 1;
    while (cap < (size_t)(n * 2 + 2))
        cap <<= 1;
    size_t mask = cap - 1;
    Entry *table = (Entry *)malloc(cap * sizeof(Entry));
    for (size_t i = 0; i < cap; ++i) {
        table[i].used = 0;
    }

    seq_t prev = (seq_t)0;
    for (idx_t step = 0; step < n; ++step) {
        uint64_t key = (uint64_t)prev;

        /* Find or create table entry for 'prev' */
        size_t idx = hash64(key) & mask;
        while (table[idx].used && table[idx].key != key) {
            idx = (idx + 1) & mask;
        }
        if (!table[idx].used) {
            table[idx].used = 1;
            table[idx].key   = key;
            table[idx].cnt   = 0;
        }

        unsigned k = table[idx].cnt;          /* bit to toggle */
        seq_t next = prev ^ (((seq_t)1) << k);
        table[idx].cnt = k + 1;                /* update counter for this state */

        prev = next;
    }

    free(table);
    return prev;
}
