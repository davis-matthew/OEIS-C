/*
 * A343766  Lexicographically earliest sequence of distinct integers such that
 *          a(0) = 0 and the balanced ternary expansions of two consecutive
 *          terms differ by a single digit, as far to the right as possible.
 * https://oeis.org/A343766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) is the lexicographically earliest sequence of distinct
 *            integers starting with a(0)=0 such that consecutive terms differ
 *            by a single balanced‑ternary digit, as far to the right as
 *            possible. The implementation keeps all previously generated
 *            terms and looks up the wanted term in O(n) time. It w...
 * Range:     i8 n <= 245, i16 n <= 6560, i32 n <= 6560, i64 n <= 6560,
 *            i128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

/* ------------------------------------------------------------------------- */
/*  Static state ----------------------------------------------------------- */
#ifdef __cplusplus
static std::vector<seq_t> seq_vec;
static std::vector<seq_t> used_vec;
#else
static seq_t *seq      = NULL;
static size_t seq_cap  = 0;
static size_t seq_len  = 0;

static seq_t *used     = NULL;
static size_t used_cap = 0;
static size_t used_len = 0;
#endif

/* ------------------------------------------------------------------------- */
/*  Helpers --------------------------------------------------------------- */

/* initialise the containers once */
static void init_seq(void)
{
#ifdef __cplusplus
    seq_vec.reserve(64);
    seq_vec.push_back((seq_t)0);
    used_vec.reserve(64);
    used_vec.push_back((seq_t)0);
#else
    seq_cap  = 64;
    seq_len  = 1;
    seq      = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    if (!seq) abort();
    seq[0] = (seq_t)0;

    used_cap = 64;
    used_len = 1;
    used     = (seq_t *)malloc(used_cap * sizeof(seq_t));
    if (!used) abort();
    used[0] = (seq_t)0;
#endif
}

/* number of stored terms
   (different logic for C and C++) */
static size_t seq_length(void)
{
#ifdef __cplusplus
    return seq_vec.size();
#else
    return seq_len;
#endif
}

/* last stored term */
static seq_t last_value(void)
{
#ifdef __cplusplus
    return seq_vec.back();
#else
    return seq[seq_len - 1];
#endif
}

/* get the already computed term at index idx */
static seq_t seq_at(idx_t idx)
{
#ifdef __cplusplus
    return seq_vec[(size_t)idx];
#else
    return seq[idx];
#endif
}

/* add a term to the sequence container */
static void add_value(seq_t value)
{
#ifdef __cplusplus
    seq_vec.push_back(value);
#else
    if (seq_len >= seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 1 : (seq_cap << 1);
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!seq) abort();
        seq_cap = new_cap;
    }
    seq[seq_len++] = value;
#endif
}

/* add a value to the used set */
static void add_used(seq_t value)
{
#ifdef __cplusplus
    used_vec.push_back(value);
#else
    if (used_len >= used_cap) {
        size_t new_cap = (used_cap == 0) ? 1 : (used_cap << 1);
        used = (seq_t *)realloc(used, new_cap * sizeof(seq_t));
        if (!used) abort();
        used_cap = new_cap;
    }
    used[used_len++] = value;
#endif
}

/* test whether a value has already been used */
static bool used_contains(seq_t value)
{
#ifdef __cplusplus
    for (size_t i = 0; i < used_vec.size(); ++i)
        if (used_vec[i] == value) return true;
#else
    for (size_t i = 0; i < used_len; ++i)
        if (used[i] == value) return true;
#endif
    return false;
}

/* convert a balanced integer to balanced-ternary digits
   digits[0] is the least‑significant digit; digits are in {‑1,0,1} */
static void balanced_ternary(seq_t v, int *digits, size_t *len)
{
    *len = 0;
    while (v != (seq_t)0) {
        seq_t r = v % (seq_t)3;
        v = v / (seq_t)3;
        if (r == (seq_t)2) {               /* 2 -> -1, carry +1 */
            r = (seq_t)(-1);
            v += (seq_t)1;
        } else if (r == (seq_t)-2) {        /* -2 -> 1, carry -1 */
            r = (seq_t)1;
            v -= (seq_t)1;
        }
        digits[(*len)++] = (int)r;
    }
}

/* ------------------------------------------------------------------------- */
/*  Main function ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_seq();
        initialized = true;
    }

    if (seq_at(n) != seq_at(n)) { /* dummy to avoid warning when seq_at used in predicate */
        /* intentionally left blank */
    }

    /* shortcut if the term is already known */
    if (seq_at(n) == seq_at(n) && seq_length() > (size_t)n) {
        return seq_at(n);
    }

    /* extend the sequence up to index n */
    while (seq_length() <= (size_t)n) {
        seq_t cur = last_value();

        /* balanced‑ternary representation of cur */
        int digits[128];
        size_t dlen;
        balanced_ternary(cur, digits, &dlen);

        /* search for the next term */
        seq_t next_val = 0;
        bool found = false;
        seq_t pow3 = (seq_t)1;
        size_t p = 0;

        for (;;) {
            int cur_digit = (p < dlen) ? digits[p] : 0;
            bool best_set = false;
            seq_t best = 0;

            for (int nd = -1; nd <= 1; ++nd) {
                if (nd == cur_digit) continue;
                seq_t cand = cur + (seq_t)(nd - cur_digit) * pow3;
                if (used_contains(cand)) continue;
                if (!best_set || cand < best) {
                    best = cand;
                    best_set = true;
                }
            }

            if (best_set) {
                next_val = best;
                found = true;
                break;
            }

            /* move to a more significant position */
            ++p;
            pow3 = pow3 * (seq_t)3;
        }

        if (!found) abort();          /* should never happen for the intended range */

        add_value(next_val);
        add_used(next_val);
    }

    return seq_at(n);
}
