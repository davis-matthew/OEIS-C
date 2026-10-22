/*
 * A334301  Irregular triangle read by rows where row k is the k-th integer
 *          partition, if partitions are sorted first by sum, then by length,
 *          and finally lexicographically.
 * https://oeis.org/A334301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth part of integer partitions sorted by sum, then
 *            by length, and finally lexicographically (Abramowitz–Stegun
 *            order)
 * Range:     u8 n <= 3615, u16 n <= 3615, u32 n <= 3615, u64 n <= 3615,
 *            u128 n <= 3615, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation generates needed partitions incrementally, so that any call
   a(n) can be answered in O(n) time after at most O(n) work each time a new
   term is required. */
#include <stdlib.h>
#include <stddef.h>

/* Static cache of previously computed terms. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_capacity = 0;

/* Current state of the incremental generator. */
static idx_t gen_sum = 1;   /* current total sum being processed */
static idx_t gen_len = 1;   /* current length of partitions within that sum */

/* Temporary array used by the recursive partition generator. */
static int cur_parts[2000];

/* Ensure the cache has room for at least one more element. */
static void push_cache(seq_t val)
{
    if (cache_size == cache_capacity) {
        size_t new_cap = (cache_capacity ? 2 * cache_capacity : 64);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_capacity = (idx_t)new_cap;
    }
    cache[cache_size++] = val;
}

/* Recursive helper: generate all partitions of 'sum' into 'len' parts.
   The parts are placed in cur_parts[pos .. pos+len-1], each part <= maxPart,
   and the sequence is produced in lexicographic (ascending) order. */
static void gen_len_partitions(int sum, int len, int pos, int maxPart)
{
    if (len == 0) {
        if (sum == 0) {
            for (int i = 0; i < pos; ++i)
                push_cache((seq_t)cur_parts[i]);
        }
        return;
    }

    /* Minimum part that can remain so that the rest can be filled with ones. */
    int minPart = (sum + len - 1) / len;   /* ceil(sum / len) */
    if (minPart < 1) minPart = 1;
    int maxAllowed = maxPart;
    if (maxAllowed > sum) maxAllowed = sum;

    for (int part = minPart; part <= maxAllowed; ++part) {
        cur_parts[pos] = part;
        gen_len_partitions(sum - part, len - 1, pos + 1, part);
    }
}

/* Generate terms until we have at least 'target' elements in cache. */
static void generate_until(idx_t target)
{
    while (cache_size < target) {
        gen_len_partitions((int)gen_sum, (int)gen_len, 0, (int)gen_sum);
        ++gen_len;
        if (gen_len > gen_sum) {
            ++gen_sum;
            gen_len = 1;
        }
    }
}

/* Main function requested by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, but safe return */
    generate_until(n);
    return cache[n - 1];
}
