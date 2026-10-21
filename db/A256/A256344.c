/*
 * A256344  Moduli n for which A248218(n) = 4 (length of the terminating cycle
 *          of 0 under x -> x^2+1 modulo n).
 * https://oeis.org/A256344
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 2470, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A256344: nth modulus m for which the orbit of 0 under x -> x^2+1 (mod m)
 * has a cycle of length 4.  The function computes the terms on demand
 * by testing successive integers with Floyd’s cycle‑finding algorithm. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/mod_mul.h"
#include "../utils/seq_realloc.h"

/* Compute f(x) = (x^2 + 1) mod m, avoiding overflow by modular multiplication. */
static uint64_t f_func(uint64_t x, uint64_t m)
{
    uint64_t t = mod_mul(x, x, m);
    t += 1;
    if (t >= m) t -= m;
    return t;
}

/* Return the length of the cycle reached by iterating x -> x^2+1 starting from 0,
 * modulo the given m.  Implementation uses Floyd’s cycle‑finding algorithm. */
static uint64_t cycle_len_u64(uint64_t m)
{
    /* Handle trivial modulus */
    if (m == 1) return 1;

    /* Phase 1: find a meeting point within the cycle */
    uint64_t tortoise = f_func(0, m);
    uint64_t hare = f_func(f_func(0, m), m);
    while (tortoise != hare) {
        tortoise = f_func(tortoise, m);
        hare = f_func(f_func(hare, m), m);
    }

    /* Phase 2: determine the cycle length λ */
    uint64_t lam = 1;
    uint64_t hare2 = f_func(tortoise, m);
    while (tortoise != hare2) {
        hare2 = f_func(hare2, m);
        ++lam;
        /* If the cycle is longer than 4 we can stop early, but keep the
         * full computation because the early exit could miss the true cycle
         * length for λ = 4. */
    }
    return lam;
}

/* The main sequence function */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;
    static uint64_t checked = 0;          /* highest modulus examined so far */

    /* Generate terms until we have at least n of them */
    while (terms_len < (size_t)n) {
        uint64_t m = checked + 1;
        if (cycle_len_u64(m) == 4) {
            /* Ensure capacity */
            if (terms_len + 1 > terms_cap) {
                size_t new_cap = terms_cap == 0 ? 16 : terms_cap << 1;
                terms = seq_realloc(terms, terms_cap, new_cap);
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)m;
        }
        ++checked;
    }

    return terms[n-1];
}
