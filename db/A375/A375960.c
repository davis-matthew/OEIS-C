/*
 * A375960  Numbers whose product of proper divisors is a cube.
 * https://oeis.org/A375960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A375960: Numbers whose product of proper divisors is a cube.
 * A positive integer m satisfies this property iff for every prime
 * exponent α in its factorisation, α·(τ(m)–2) ≡ 0 (mod 6),
 * where τ(m) is the number of divisors of m (τ(m)=∏(α+1)). */

/* The build system provides the types seq_t and idx_t.  Do not override them. */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t, uint64_t *, uint64_t *, size_t *); */

/* ------- internal helpers --------------------------------------- */
static bool is_term(uint64_t m)
{
    if (m == 1)                   /* 1 is included trivially */
        return true;

    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(m, primes, exps, &cnt);   /* factorisation (m ≤ 2^64-1) */

    /* τ(m) modulo 6:  τ = ∏ (α_i + 1) */
    uint64_t tau_mod6 = 1 % 6;
    for (size_t i = 0; i < cnt; ++i) {
        tau_mod6 = (tau_mod6 * ((exps[i] + 1) % 6)) % 6;
    }

    /* (τ–2) modulo 6 */
    uint64_t t_minus_2 = (tau_mod6 + 6 - 2) % 6;

    /* Verify α_i·t_minus_2 ≡ 0 (mod 6) for all exponents */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t a_mod6 = exps[i] % 6;
        if ((a_mod6 * t_minus_2) % 6 != 0)
            return false;
    }
    return true;
}

static void append_term(uint64_t val, seq_t **array, size_t *len, size_t *cap)
{
    if (*len == *cap) {
        size_t new_cap = *cap ? *cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(*array, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* unrecoverable allocation failure */
        *array = tmp;
        *cap = new_cap;
    }
    (*array)[*len] = (seq_t)val;
    ++(*len);
}

/* ------- public sequence function -------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)               /* behaviour below offset is irrelevant */
        return 0;

    /* Static state for memoised terms and candidate scan */
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static idx_t last_candidate = 1;   /* next integer to test */

    /* Generate terms until we have at least n terms cached */
    while (seq_len < (size_t)n) {
        if (is_term((uint64_t)last_candidate))
            append_term((uint64_t)last_candidate, &seq, &seq_len, &seq_cap);
        ++last_candidate;
    }
    return seq[n - 1];
}
