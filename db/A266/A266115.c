/*
 * A266115  Numbers which have smaller siblings in A263267-tree: numbers n for
 *          which there exists some k < n such that k - d(k) = n - d(n), where
 *          d(n) = A000005(n), the number of divisors of n.
 * https://oeis.org/A266115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This function implements the OEIS sequence A266115:
 *  a(n) = the n-th integer m such that there exists k < m
 *  with k - d(k) = m - d(m), where d(x) is the divisor count.
 *  The algorithm builds a hash table of values m - d(m) seen so far;
 *  when a value is encountered for the second time, the corresponding
 *  m is a member of the sequence.  The function returns the n‑th such
 *  member.  It works for any `n >= 1` and for any order of calls. */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* ---------- divisor count routine (trial division) ---------- */
static inline uint64_t my_divisor_count(uint64_t n)
{
    uint64_t count = 1;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            int e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            count *= (uint64_t)(e + 1);
        }
    }
    if (n > 1) /* remaining prime factor */
        count *= 2;
    return count;
}

/* ---------- hash table for m - d(m) values ---------- */
typedef struct {
    idx_t key;        /* m - d(m) */
    uint16_t count;   /* 0, 1, or >= 2 (we only care about >1) */
} ht_entry_t;

/* simple multiplicative hash */
static inline idx_t ht_hash(idx_t key, idx_t mask)
{
    key ^= key >> 33;
    key ^= key >> 27;
    key ^= key >> 12;
    return key & mask;
}

/* find slot for key; if absent insert with count = 0 */
static idx_t ht_find_or_insert(ht_entry_t *tab, unsigned char *used,
                               size_t sz, idx_t key)
{
    size_t mask = sz - 1;
    idx_t pos = ht_hash(key, mask);
    for (;;) {
        if (!used[pos]) {
            used[pos] = 1;
            tab[pos].key = key;
            tab[pos].count = 0;
            return pos;
        }
        if (tab[pos].key == key)
            return pos;
        pos = (pos + 1) & mask;
    }
}

/* ---------- main sequence routine ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1 but safe */

    /* choose table size as power of two, load factor <= 0.5 */
    size_t tbl_size = 1;
    while (tbl_size < (size_t)(4 * n)) tbl_size <<= 1;

    ht_entry_t *table = (ht_entry_t *)calloc(tbl_size, sizeof(ht_entry_t));
    unsigned char *used  = (unsigned char *)calloc(tbl_size, 1);
    if (!table || !used) {
        free(table); free(used);
        return (seq_t)0;
    }

    idx_t found = 0;          /* how many sequence members found */
    uint64_t cur  = 0;        /* current integer being tested */

    while (found < n) {
        ++cur;
        uint64_t d = my_divisor_count(cur);
        idx_t key = (idx_t)(cur - d);   /* m - d(m) fits in idx_t */

        idx_t pos = ht_find_or_insert(table, used, tbl_size, key);

        if (table[pos].count > 0) {         /* already seen before */
            ++found;
            if (found == n) {
                free(table); free(used);
                return (seq_t)cur;
            }
        }
        ++table[pos].count;
    }

    /* should never reach here */
    free(table); free(used);
    return (seq_t)0;
}
