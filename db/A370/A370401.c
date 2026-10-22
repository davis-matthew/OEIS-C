/*
 * A370401  Lexicographically earliest sequence of distinct nonnegative terms
 *          such that the first digit of a(n) is present in a(n+1).
 * https://oeis.org/A370401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 215, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  LS:  Lexicographically smallest sequence of distinct non‑negative
    integers such that the first digit of a(n) occurs in a(n+1).
    The sequence starts with a(1)=0.  The algorithm uses a lazy
    generator with memoisation; it works for any seq_t (unsigned
    integer or arbitrary‑precision) and any idx_t.  It is O(n) in
    sequence length.  */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Global tables */
static seq_t *seq = NULL;          /* generated terms */
static size_t seq_cap = 0;         /* number of terms stored */

static unsigned char *used = NULL; /* used flag array */
static size_t used_cap = 0;        /* capacity of 'used' */

static seq_t minUnused[10];       /* smallest unused number containing each digit */

/* ------------------------------------------------------------------ */
/* Ensure used array can address index n (or any number < n).  */
static void ensure_used_capacity(seq_t n)
{
    size_t val = (size_t)n;
    if (val < used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 1;
    while (new_cap <= val) new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
    if (!tmp) exit(1);
    memset(tmp + used_cap, 0, new_cap - used_cap);
    used = tmp;
    used_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* True if decimal representation of x contains digit d. */
static bool contains_digit(seq_t x, int d)
{
    if (x == 0) return d == 0;
    while (x > 0) {
        int dig = (int)(x % 10);
        if (dig == d) return true;
        x /= 10;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Return the leftmost decimal digit of x. */
static int first_digit(seq_t x)
{
    if (x == 0) return 0;
    while (x >= 10) x /= 10;
    return (int)x;
}

/* ------------------------------------------------------------------ */
/* Find the smallest unused number >= start that contains digit d. */
static seq_t find_next_unused_digit(int d, seq_t start)
{
    for (seq_t cand = start;; ++cand) {
        ensure_used_capacity(cand);
        if (!used[(size_t)cand] && contains_digit(cand, d))
            return cand;
    }
}

/* ------------------------------------------------------------------ */
/* Initialise static data structures on first call. */
static void init_sequence(void)
{
    /* used bitset */
    used_cap = 1;
    used = (unsigned char *)calloc(used_cap, sizeof(unsigned char));
    used[0] = 1;              /* a(1)=0 is used */

    /* first term */
    seq_cap = 1;
    seq = (seq_t *)calloc(1, sizeof(seq_t));
    seq[0] = (seq_t)0;        /* a(1)=0 */

    /* initialise minUnused for digits 0‑9 */
    for (int d = 0; d <= 9; ++d)
        minUnused[d] = find_next_unused_digit(d, 0);
}

/* ------------------------------------------------------------------ */
/* Return the n-th term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* offset starts at 1; this case never occurs */

    if (!seq) init_sequence();

    if (seq_cap < (size_t)n) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 1;
        if (new_cap < (size_t)n) new_cap = n;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));

        for (size_t i = seq_cap; i < (size_t)n; ++i) {
            seq_t cur = seq[i - 1];
            int d = first_digit(cur);
            seq_t nxt = minUnused[d];
            seq[i] = nxt;

            /* mark as used */
            ensure_used_capacity(nxt);
            used[(size_t)nxt] = 1;

            /* update minUnused for all digits present in nxt */
            seq_t tmp = nxt;
            if (tmp == 0) {
                if (minUnused[0] == nxt)
                    minUnused[0] = find_next_unused_digit(0, nxt + 1);
            } else {
                while (tmp > 0) {
                    int dig = (int)(tmp % 10);
                    if (minUnused[dig] == nxt)
                        minUnused[dig] = find_next_unused_digit(dig, nxt + 1);
                    tmp /= 10;
                }
            }
        }
        seq_cap = n;
    }

    return seq[n - 1];
}
