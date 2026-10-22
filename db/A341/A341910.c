/*
 * A341910  Lexicographically earliest sequence of distinct nonnegative
 *          integers such that for any n >= 0, the number of runs in the
 *          binary expansion of n equals the number of ones in the binary
 *          expansion of a(n).
 * https://oeis.org/A341910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The OEIS formula is not a simple closed form.  The sequence is
    defined as follows:
      a(n) is the lexicographically smallest unused non‑negative
      integer whose binary popcount equals the number of runs in the
      binary expansion of n.  A “run” is a maximal block of equal
      consecutive bits; for n = 0 the run count is 0.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- popcount for any unsigned type seq_t ---------- */
static inline unsigned bitcount(seq_t x)
{
    unsigned c = 0;
    while (x) {
        x &= x - 1;          /* drop lowest set bit */
        ++c;
    }
    return c;
}

/* ---------- number of runs in binary expansion of n ---------- */
static inline unsigned long long run_count(idx_t n)
{
    if (n == 0) return 0;    /* by definition */
    unsigned long long cnt = 0;
    int prev = -1;           /* sentinel for "no previous bit" */
    while (n) {
        int bit = n & 1U;
        if (bit != prev) {
            ++cnt;
            prev = bit;
        }
        n >>= 1U;
    }
    return cnt;
}

/* ---------- static state for memoization --------------------- */
#define MAX_RUN 64            /* safe upper bound for run counts */
static seq_t *seq_vals = NULL;   /* array of a(i) values */
static size_t seq_len   = 0;
static size_t seq_cap   = 0;
static uint8_t *used    = NULL;   /* bitmap of values that have
                                    already appeared */
static size_t used_cap  = 0;
static seq_t next_candidate[MAX_RUN + 1] = {0};  /* next unchecked value per run */

/* ---------- ensure bitmap big enough for value val ------------- */
static void ensure_used_capacity(seq_t val)
{
    uint64_t uval = (uint64_t)val;          /* works for n ≤ 10000 */
    size_t needed_bytes = (size_t)(uval >> 3) + 1;

    while (needed_bytes > used_cap) {
        size_t new_cap = used_cap ? used_cap << 1 : 64;
        if (new_cap < needed_bytes) new_cap = needed_bytes;
        uint8_t *tmp = (uint8_t *)realloc(used, new_cap);
        if (!tmp) abort();
        memset(tmp + used_cap, 0, new_cap - used_cap);
        used = tmp;
        used_cap = new_cap;
    }
}

/* ---------- generate terms up to index n --------------------- */
static void ensure_terms(idx_t n)
{
    if (!seq_vals) {                    /* first call – initialise */
        seq_cap = 32;
        seq_vals = (seq_t *)calloc(seq_cap, sizeof(seq_t));
        if (!seq_vals) abort();

        used_cap = 64;
        used = (uint8_t *)calloc(used_cap, sizeof(uint8_t));
        if (!used) abort();
    }

    while (seq_len <= (size_t)n) {
        unsigned run = (unsigned)run_count(seq_len);
        seq_t cand = next_candidate[run];

        /* scan forward until we find an unused candidate with
           matching popcount */
        for (;;) {
            ensure_used_capacity(cand);
            uint64_t uval = (uint64_t)cand;
            size_t byte = uval >> 3;
            unsigned mask = 1U << (uval & 7U);
            if (!(used[byte] & mask) && bitcount(cand) == run)
                break;                /* found a valid candidate */
            ++cand;
        }

        /* store the value */
        if (seq_len == seq_cap) {
            seq_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(seq_vals, seq_cap * sizeof(seq_t));
            if (!tmp) abort();
            seq_vals = tmp;
        }
        seq_vals[seq_len++] = cand;

        /* mark used */
        uint64_t uval = (uint64_t)cand;
        size_t byte = uval >> 3;
        unsigned mask = 1U << (uval & 7U);
        used[byte] |= mask;

        /* next search starts after the value we just used */
        next_candidate[run] = cand + 1;
    }
}

/* ---------- public sequence function --------------------------- */
seq_t a(idx_t n)
{
    ensure_terms(n);
    return seq_vals[n];
}
