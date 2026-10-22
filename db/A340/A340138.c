/*
 * A340138  a(1)=0, a(n+1) = the least m not already in the sequence such that
 *          the decimal digital root r of a(n-1) is found somewhere among the
 *          decimal digits of m.
 * https://oeis.org/A340138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 252, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A340138
 *
 *  a(1) = 0
 *  For n ≥ 2 : a(n) = the least m not yet used such that the
 *  decimal digital root of a(n‑1) appears among the decimal digits of m.
 *
 *  The implementation uses a bitmap of used numbers together with a
 *  pointer per digit (0–9).  For each digit r the pointer stores the
 *  smallest unused number that might contain the digit r; the pointer
 *  is advanced until an unused candidate with that digit is found.
 *  This guarantees that the chosen m is the minimal one satisfying the
 *  condition, independent of the order of calls to a(n).  The approach
 *  runs in essentially linear time for n up to 10000.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/*--- Internal state ----------------------------------------------------*/
static seq_t *seq       = NULL;   /* sequence values                */
static size_t seq_cap   = 0;      /* capacity of seq array          */
static size_t seq_len   = 0;      /* number of values stored        */

static uint8_t *used     = NULL;   /* bitmap: 1 = already used       */
static size_t used_cap   = 0;      /* capacity of used bitmap        */

static seq_t next_unused[10];      /* pointer for each digit 0‑9    */

/*--- Helpers -----------------------------------------------------------*/
static inline seq_t digital_root(seq_t x)
{
    /* 0 has digital root 0; otherwise 1 + ((x‑1) mod 9) */
    if (x == (seq_t)0) return (seq_t)0;
    return (seq_t)(1 + ((x - (seq_t)1) % (seq_t)9));
}

static inline bool contains_digit(seq_t m, int r)
{
    if (m == (seq_t)0) return r == 0;
    while (m > (seq_t)0) {
        int d = (int)(m % (seq_t)10);
        if (d == r) return true;
        m /= (seq_t)10;
    }
    return false;
}

static void ensure_seq_cap(size_t min_cap)
{
    if (seq_cap >= min_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 1;
    while (new_cap < min_cap) new_cap *= 2;
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!seq) abort();
    seq_cap = new_cap;
}

static void ensure_used_cap(size_t min_cap)
{
    if (used_cap >= min_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 1;
    while (new_cap < min_cap) new_cap *= 2;
    used = (uint8_t *)realloc(used, new_cap);
    if (!used) abort();
    /* initialise fresh part to zero */
    for (size_t i = used_cap; i < new_cap; ++i) used[i] = 0;
    used_cap = new_cap;
}

/*--- Sequence initialisation ------------------------------------------*/
static void init_sequence(void)
{
    if (seq_len != 0) return;               /* already initialised */
    ensure_seq_cap(1);
    ensure_used_cap(1);

    seq[0] = (seq_t)0;            /* a(1) = 0 */
    used[0] = 1;
    seq_len = 1;
    for (int d = 0; d < 10; ++d) next_unused[d] = (seq_t)0;
}

/*--- Generate terms up to (but not including) target length -----------*/
static void compute_to(size_t target)
{
    if (seq_len >= target) return;

    while (seq_len < target) {
        seq_t prev = seq[seq_len - 1];
        int r = (int)digital_root(prev);

        seq_t cand = next_unused[r];

        /* advance until we find an unused candidate with digit r */
        while (true) {
            if ((size_t)cand >= used_cap) ensure_used_cap((size_t)cand + 1);
            if (!used[(size_t)cand] && contains_digit(cand, r)) break;
            ++cand;
        }

        /* record the chosen term */
        used[(size_t)cand] = 1;
        next_unused[r] = cand + (seq_t)1;

        /* extend seq array if necessary */
        if (seq_len + 1 > seq_cap) ensure_seq_cap(seq_len + 1);

        seq[seq_len] = cand;
        ++seq_len;
    }
}

/*--- Public function ---------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < (idx_t)1) abort();   /* undefined for n < 1 */

    size_t target = (size_t)n;   /* desired length of the sequence */

    init_sequence();
    compute_to(target);
    return seq[target - 1];
}
