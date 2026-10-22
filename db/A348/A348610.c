/*
 * A348610  Number of alternating ordered factorizations of n.
 * https://oeis.org/A348610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of alternating ordered factorizations of n
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Helper table for memoisation */
#define HASH_TABLE_BITS 20
#define HASH_TABLE_SIZE ((size_t)1 << HASH_TABLE_BITS)
#define HASH_MASK (HASH_TABLE_SIZE - 1)

typedef struct {
    idx_t prod;
    idx_t prev;
    unsigned char dir;
    unsigned char used;
    seq_t val;
} memo_entry;

static memo_entry memo_table[HASH_TABLE_SIZE];

/* Hash of a memoisation key */
static inline size_t hash_memo(idx_t prod, idx_t prev, unsigned char dir)
{
    uint64_t h = prod * 11995408973635179863ULL;
    h ^= prev * 1000000007ULL;
    h ^= (uint64_t)(dir + 1);
    return (size_t)h & HASH_MASK;
}

/* Find an entry, or NULL if missing */
static memo_entry *lookup_memo(idx_t prod, idx_t prev, unsigned char dir)
{
    size_t h = hash_memo(prod, prev, dir);
    for (size_t i = h; ; i = (i + 1) & HASH_MASK) {
        if (!memo_table[i].used) return NULL;
        if (memo_table[i].prod == prod &&
            memo_table[i].prev == prev &&
            memo_table[i].dir  == dir)
            return &memo_table[i];
    }
}

/* Insert a value into the memoisation table */
static void insert_memo(idx_t prod, idx_t prev, unsigned char dir, seq_t val)
{
    size_t h = hash_memo(prod, prev, dir);
    for (size_t i = h; ; i = (i + 1) & HASH_MASK) {
        if (!memo_table[i].used) {
            memo_table[i].prod = prod;
            memo_table[i].prev = prev;
            memo_table[i].dir  = dir;
            memo_table[i].used = 1;
            memo_table[i].val  = val;
            return;
        }
        if (memo_table[i].prod == prod &&
            memo_table[i].prev == prev &&
            memo_table[i].dir  == dir) {
            memo_table[i].val = val;
            return;
        }
    }
}

/* Recursive counting of alternating factorizations:
   prod – product to factorise
   prev – previous factor in the sequence
   dir  – 0 : next factor must be > prev, 1 : next factor must be < prev
*/
static seq_t count_seq(idx_t prod, idx_t prev, unsigned char dir)
{
    if (prod <= 1) return 0;
    memo_entry *me = lookup_memo(prod, prev, dir);
    if (me) return me->val;

    seq_t total = 0;

    /* iterate proper divisors f < prod */
    for (idx_t i = 2; i <= prod / i; ++i) {
        if (prod % i) continue;
        idx_t f1 = i;
        idx_t f2 = prod / i;

        if ((dir == 0 && f1 > prev) || (dir == 1 && f1 < prev)) {
            idx_t next = prod / f1;   /* equals f2 */
            if (next == 1) total += 1;
            else total += count_seq(next, f1, dir ^ 1);
        }

        if (f2 != f1) {
            if ((dir == 0 && f2 > prev) || (dir == 1 && f2 < prev)) {
                idx_t next = prod / f2;   /* equals f1 */
                if (next == 1) total += 1;
                else total += count_seq(next, f2, dir ^ 1);
            }
        }
    }

    /* divisor equal to prod itself (finishes the chain) */
    if ((dir == 0 && prod > prev) || (dir == 1 && prod < prev))
        total += 1;

    insert_memo(prod, prev, dir, total);
    return total;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;                 /* empty factorisation */

    seq_t result = 1;                     /* single-factor factorisation [n] */

    for (idx_t i = 2; i <= n / i; ++i) {
        if (n % i) continue;
        idx_t d1 = i;
        idx_t d2 = n / i;

        if (d1 < n) {
            result += count_seq(n / d1, d1, 0);
            result += count_seq(n / d1, d1, 1);
        }
        if (d2 != d1 && d2 < n) {
            result += count_seq(n / d2, d2, 0);
            result += count_seq(n / d2, d2, 1);
        }
    }

    return result;
}
