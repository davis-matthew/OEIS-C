/*
 * A342498  Number of integer partitions of n with strictly increasing first
 *          quotients.
 * https://oeis.org/A342498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = number of integer partitions of n with strictly
 *            increasing * first quotients (q_i = a_{i+1}/a_i). The
 *            computation is done by a * recursive dynamic programming that
 *            keeps track of the last two parts * of the partition. The
 *            recursion is memoized using a simple * open-addressing hash
 *            table...
 * Range:     u8 n <= 47, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* hash table size – power of two for modulo via bitmask */
#define MEMO_CAP (1 << 21)   /* enough for ~2 million states */

/* hash table entry on each slot */
typedef struct {
    idx_t  r;   /* remaining sum */
    idx_t  a;   /* previous part */
    idx_t  b;   /* current part (last part added) */
    seq_t  val; /* number of ways from this state */
    unsigned char used;   /* 0 = empty, 1 = used */
} memo_entry_t;

/* static hash table */
static memo_entry_t memo_table[MEMO_CAP];

/* simple mixed hash function */
static inline size_t memo_hash(idx_t r, idx_t a, idx_t b)
{
    /* use big prime multipliers – works for 32/64‑bit values */
    size_t h = (size_t)r * 1315423911u;
    h ^= (size_t)a * 2654435761u;
    h ^= (size_t)b * 9756100u;
    return h & (MEMO_CAP - 1);
}

/* lookup value in the memo table
   return 1 and set out if found, otherwise 0. */
static int memo_get(idx_t r, idx_t a, idx_t b, seq_t *out)
{
    size_t h = memo_hash(r, a, b);
    while (memo_table[h].used) {
        if (memo_table[h].r == r &&
            memo_table[h].a == a &&
            memo_table[h].b == b) {
            *out = memo_table[h].val;
            return 1;
        }
        h = (h + 1) & (MEMO_CAP - 1);
    }
    return 0;
}

/* store value in the memo table */
static void memo_set(idx_t r, idx_t a, idx_t b, seq_t val)
{
    size_t h = memo_hash(r, a, b);
    while (memo_table[h].used) {
        if (memo_table[h].r == r &&
            memo_table[h].a == a &&
            memo_table[h].b == b) {
            /* already stored – nothing to do */
            return;
        }
        h = (h + 1) & (MEMO_CAP - 1);
    }
    memo_table[h].used = 1;
    memo_table[h].r    = r;
    memo_table[h].a    = a;
    memo_table[h].b    = b;
    memo_table[h].val  = val;
}

/* recursive helper that returns the number of ways to finish a
 * partition when the two last parts are 'a' and 'b' and the
 * remaining sum to be distributed is 'r'. */
static seq_t count_rec(idx_t r, idx_t a, idx_t b)
{
    if (r == 0)
        return 1;                /* finished the partition */

    seq_t cached;
    if (memo_get(r, a, b, &cached))
        return cached;

    /* compute lower bound on next part:
     * c > b*b / a  =>  c_min = floor(b*b/a) + 1  */
    idx_t lo = (b * b) / a + 1;
    if (lo < 1)
        lo = 1;                 /* safety – should not happen */

    /* upper bounds */
    idx_t hi = b;                /* cannot exceed last part */
    if (hi > r)
        hi = r;                 /* cannot exceed remaining sum */

    seq_t sum = 0;
    if (lo <= hi) {
        /* iterate over all admissible next parts */
        for (idx_t c = lo; c <= hi; ++c) {
            sum += count_rec(r - c, b, c);
        }
    }

    memo_set(r, a, b, sum);
    return sum;
}

/* ------------------------------------------------------------------------- */
/* Main sequence function --------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)        /* a(0) = 1 */
        return 1;

    seq_t total = 1;   /* the partition consisting of the single part n */

    /* partitions with at least two parts */
    for (idx_t first = 1; first < n; ++first) {
        /* second part must be <= first */
        for (idx_t second = 1; second <= first; ++second) {
            idx_t sum12 = first + second;
            if (sum12 > n)
                break;   /* larger 'second' only increases the sum */

            idx_t rem = n - sum12;          /* remaining sum to distribute */
            if (rem == 0) {
                ++total;                    /* only two parts */
            } else {
                total += count_rec(rem, first, second);
            }
        }
    }
    return total;
}
