/*
 * A247143  Lexicographically earliest permutation of the natural numbers such
 *          that in decimal representation d(n) is contained in a(n), where d
 *          is the digit sequence of the concatenation of the a-sequence, cf.
 *          A247149.
 * https://oeis.org/A247143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 234, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A247143: Lexicographically earliest permutation of the natural numbers 
   such that the nth digit of the concatenation of the sequence contains
   that digit.  Algorithm: maintain the digit stream and a set of used
   numbers.  For each index n, if the nth digit is already known choose
   the smallest unused number that contains that digit; otherwise pick
   the smallest unused number.  Append the chosen number’s digits to
   the stream and mark it as used.  The state is memoised so any call
   order works.
*/
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* external types supplied by the build environment */
extern seq_t a(idx_t n);

/* ----------------- static state ----------------- */
static bool       initialized      = false;
static seq_t     *seq              = NULL;   /* sequence values          */
static idx_t      seq_len          = 0;      /* number of computed terms */
static idx_t      seq_cap          = 0;      /* allocated capacity       */

static unsigned char *digits_s     = NULL;   /* concatenated digits      */
static idx_t      digits_len        = 0;
static idx_t      digits_cap        = 0;

static bool       *used            = NULL;   /* used numbers flag        */
static idx_t      used_cap          = 0;      /* allocated capacity       */

static idx_t      next_any          = 0;      /* smallest unused number   */
static idx_t      next_digit[10]    = {0};    /* smallest unused with digit */

/* ----------------- utility ----------------- */
static bool contains_digit(idx_t num, unsigned d)
{
    if (num == 0)
        return d == 0;
    while (num > 0) {
        if (num % 10 == d) return true;
        num /= 10;
    }
    return false;
}

static void ensure_used_cap(idx_t upto)
{
    if (used_cap > upto) return;
    idx_t new_cap = used_cap == 0 ? 1 : used_cap;
    while (new_cap <= upto) new_cap <<= 1;
    bool *new_used = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!new_used) exit(1);
    if (new_cap > used_cap)
        memset(new_used + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used = new_used;
    used_cap = new_cap;
}

static void ensure_seq_cap(idx_t needed)
{
    if (seq_cap >= needed) return;
    idx_t new_cap = seq_cap == 0 ? 1 : seq_cap;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!new_seq) exit(1);
    seq = new_seq;
    seq_cap = new_cap;
}

static void ensure_digits_cap(idx_t needed)
{
    if (digits_cap >= needed) return;
    idx_t new_cap = digits_cap == 0 ? 8 : digits_cap;
    while (new_cap < needed) new_cap <<= 1;
    unsigned char *new_digits = (unsigned char *)realloc(digits_s, new_cap * sizeof(unsigned char));
    if (!new_digits) exit(1);
    digits_s = new_digits;
    digits_cap = new_cap;
}

static void append_digit(unsigned char d)
{
    if (digits_len == digits_cap)
        ensure_digits_cap(digits_len + 1);
    digits_s[digits_len++] = d;
}

/* ----------------- initialization ----------------- */
static void init_state(void)
{
    seq       = NULL; seq_len = 0; seq_cap = 0;
    digits_s  = NULL; digits_len = 0; digits_cap = 0;
    used      = NULL; used_cap   = 0;
    for (int i = 0; i < 10; ++i) next_digit[i] = 0;
    next_any = 0;
}

/* ----------------- core algorithm ----------------- */
static void extend_to(idx_t n)
{
    while (seq_len <= n) {
        /* determine required digit for this index */
        int req_digit = -1;
        if (seq_len < digits_len)
            req_digit = digits_s[seq_len];

        idx_t chosen;

        if (req_digit == -1) {          /* no requirement – choose smallest unused */
            idx_t cand = next_any;
            while (1) {
                ensure_used_cap(cand);
                if (!used[cand])
                    break;
                ++cand;
            }
            chosen = cand;
            next_any = cand + 1;
        } else {                        /* need smallest unused containing req_digit */
            idx_t cand = next_digit[req_digit];
            while (1) {
                ensure_used_cap(cand);
                if (!used[cand] && contains_digit(cand, (unsigned)req_digit))
                    break;
                ++cand;
            }
            chosen = cand;
            next_digit[req_digit] = cand + 1;
        }

        /* mark number as used */
        ensure_used_cap(chosen);
        used[chosen] = true;

        /* store the value in the sequence */
        ensure_seq_cap(seq_len + 1);
        seq[seq_len++] = (seq_t)chosen;

        /* append its digits to the global digit list */
        idx_t temp = chosen;
        if (temp == 0) {
            append_digit(0);
        } else {
            unsigned char buf[21];
            int p = 0;
            while (temp > 0) {
                buf[p++] = (unsigned char)(temp % 10);
                temp   /= 10;
            }
            for (int i = p - 1; i >= 0; --i)
                append_digit(buf[i]);
        }
    }
}

/* ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) {
        init_state();
        initialized = true;
    }

    if (seq_len <= n)
        extend_to(n);

    return seq[n];
}
