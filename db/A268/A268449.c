/*
 * A268449  Last digit of a(n) + first digit of a(n+1) occurs as a substring
 *          in a(n); lexicographic first sequence with this property and no
 *          duplicates.
 * https://oeis.org/A268449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = lexicographically first permutation of the numbers of the
 *            form 10*k+z, 0 <= z <= 9, k having a digit > z or a digit 1
 *            followed by a digit < z, such that for every n the last digit of
 *            a(n) plus the first digit of a(n+1) (interpreted as a decimal
 *            number) occurs as a substring somewhere in a(n)
 * Range:     u8 n <= 31, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_digits, sum_substring_exists, used_hash
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_digits.h"
#include "../utils/sum_substring_exists.h"
#include "../utils/used_hash.h"

/* ------------------------------------------------------------------ */
/* Hash table for used numbers (seq_t)                                 */
/* ------------------------------------------------------------------ */
static seq_t *used_table = NULL;
static size_t used_cap = 0;

static inline bool used_contains(seq_t x)
{
    size_t h = used_hash(x) & (used_cap - 1);
    while (used_table[h]) {
        if (used_table[h] == x) return true;
        h = (h + 1) & (used_cap - 1);
    }
    return false;
}

static inline void used_add(seq_t x)
{
    size_t h = used_hash(x) & (used_cap - 1);
    while (used_table[h]) h = (h + 1) & (used_cap - 1);
    used_table[h] = x;
}

/* ------------------------------------------------------------------ */
/* Sequence storage (dynamic array)                                    */
/* ------------------------------------------------------------------ */
static seq_t *seq = NULL;
static size_t seq_cap = 0;   /* total capacity */
static size_t seq_len = 0;   /* number of terms stored (1‑indexed) */

/* +1 to accommodate 1‑based indexing; seq[0] unused */
static void seq_ensure_cap(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t cap = (seq_cap > 1) ? seq_cap : 8;
    while (cap < needed) cap <<= 1;
    seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
    seq_cap = cap;
}

/* ------------------------------------------------------------------ */
/* Helpers for digit inspection                                       */

/* ------------------------------------------------------------------ */
/* Test whether a number fits the “good k” condition described in OEIS. */
static bool is_allowed(seq_t x)
{
    seq_t k = x / 10;
    seq_t z = x % 10;
    if (k == 0) return false;          /* no digits in k → condition fails */

    unsigned char d[64];
    int len = 0;
    seq_t tmp = k;
    while (tmp) {
        d[len++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    }
    for (int i = 0; i < len / 2; ++i) {
        unsigned char t = d[i];
        d[i] = d[len-1-i];
        d[len-1-i] = t;
    }

    bool cond = false;
    bool prev_one = false;
    for (int i = 0; i < len; ++i) {
        unsigned char dig = d[i];
        if (dig > z) cond = true;
        if (prev_one && dig < z) cond = true;
        prev_one = (dig == 1);
    }
    return cond;
}

/* ------------------------------------------------------------------ */
/* Generators for each leading digit e (1..9) to find smallest unused
   allowed numbers with that leading digit. */
typedef struct {
    uint8_t e;       /* leading digit (0..9) */
    seq_t k;         /* current value of k (>= e) */
    int z;           /* current suffix digit (0..9) */
    seq_t pow10;     /* 10^m where k has m+1 digits */
} EGen;
static EGen gens[10];
static bool gens_inited = false;

static void gens_init(void)
{
    for (int e = 0; e <= 9; ++e) {
        gens[e].e = (uint8_t)e;
        gens[e].k = (seq_t)e;
        gens[e].z = 0;
        gens[e].pow10 = 1;
    }
    gens_inited = true;
}

/* Advance EGen to the next number in the sequence of numbers
   that start with the same leading digit. */
static void advance_after_candidate(EGen *g)
{
    ++g->z;
    if (g->z == 10) {
        g->z = 0;
        ++g->k;
    }
    seq_t k_max = (g->e + 1) * g->pow10 - 1;
    if (g->k > k_max) {
        g->pow10 *= 10;
        g->k = g->e * g->pow10;
    }
}

/* Return the smallest unused allowed number with leading digit g->e.
   The generator's state *is* left pointing to that number (not advanced). */
static seq_t find_min_candidate(EGen *g)
{
    while (1) {
        seq_t num = g->k * 10 + g->z;
        if (!used_contains(num) && is_allowed(num)) {
            return num;
        }
        /* skip this number (used or disallowed) */
        advance_after_candidate(g);
    }
}

/* ------------------------------------------------------------------ */
/* Main routine: compute a(n) by extending the sequence as needed */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* unused but harmless */

    /* Initialise tables on first call */
    if (!used_cap) {
        used_cap = 1 << 18;          /* 262144 → plenty for 10‑thousands terms */
        used_table = (seq_t *)calloc(used_cap, sizeof(seq_t));
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len = 1;                 /* a(1) is pre‑filled */
        seq[1] = 10;
        used_add(10);
        gens_init();
    }

    /* Extend sequence up to requested index */
    while (seq_len < (size_t)n) {
        seq_t cur = seq[seq_len];          /* a(seq_len) */
        unsigned char cur_digits[64];
        int cur_len = 0;
        factor_digits(cur, cur_digits, &cur_len);

        uint8_t d = (uint8_t)(cur % 10);

        seq_t best = 0;
        bool has = false;
        int best_e = -1;

        for (int e = 1; e <= 9; ++e) {
            int sum = d + e;
            if (!sum_substring_exists(cur_digits, cur_len, sum)) continue;

            seq_t cand = find_min_candidate(&gens[e]);
            if (!has || cand < best) {
                best = cand;
                has  = true;
                best_e = e;
            }
        }
        /* best must exist for valid OEIS sequence */
        if (!has) return 0;  /* defensive, should not happen */

        /* Append to sequence and mark used */
        seq_ensure_cap(seq_len + 2);
        seq[++seq_len] = best;
        used_add(best);

        /* Advance generator of the chosen leading digit */
        advance_after_candidate(&gens[best_e]);
    }

    return seq[(size_t)n];
}
