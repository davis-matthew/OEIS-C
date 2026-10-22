/*
 * A398710  Numbers > 100 such that all the substrings of length = 2 are
 *          primes and are all distinct.
 * https://oeis.org/A398710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1907, u64 n <= 6296, u128 n <= 6296,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n), the nth number >100 whose every consecutive two‑digit
   substring is a prime and all such substrings are distinct.
   The sequence is obtained by exploring all directed trails in a
   graph whose vertices are the decimal digits and whose edges are the
   21 two‑digit primes.  Every trail of length ≥2 yields a valid number.
   The values are generated once, sorted in ascending order, and
   returned by index.  The implementation uses a fixed static array
   large enough for the 6296 known terms.  This works for both the
   C and the C++ (arbitrary‑precision) builds. */

#include <stdlib.h>
#include <stdbool.h>

static const struct {int from; int to;} edges[21] = {
    {1,1},{1,3},{1,7},{1,9},
    {2,3},{2,9},
    {3,1},{3,7},
    {4,1},{4,3},{4,7},
    {5,3},{5,9},
    {6,1},{6,7},
    {7,1},{7,3},{7,9},
    {8,3},{8,9},
    {9,7}
};
#define EDGE_COUNT 21

/* maximum possible terms (known to be 6296) + safety margin */
#define MAX_TERMS 7000

static seq_t terms[MAX_TERMS];
static size_t terms_len = 0;

/* comparison function for qsort */
static int cmp_seq(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t*)p1;
    seq_t b = *(const seq_t*)p2;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}

/* Append value v to the static array; caller guarantees capacity. */
static void push(seq_t v)
{
    if (terms_len < MAX_TERMS)
        terms[terms_len++] = v;
}

/* Depth‑first search building directed trails.
   cur_digit: current last digit in the trail
   cur_num:   current number formed by the trail
   used:      bitmask of edges already used
   used_cnt:  number of edges used so far
*/
static void dfs(int cur_digit, seq_t cur_num, unsigned int used,
                int used_cnt)
{
    if (used_cnt >= 2)
        push(cur_num);

    if (used_cnt == 11)   /* longest trails have 11 edges (12 digits) */
        return;

    for (int i = 0; i < EDGE_COUNT; ++i) {
        if (!(used & (1u << i)) && edges[i].from == cur_digit) {
            int to = edges[i].to;
            dfs(to,
                cur_num * 10 + (seq_t)to,
                used | (1u << i),
                used_cnt + 1);
        }
    }
}

/* Memoisation flag */
static bool terms_built = false;

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (!terms_built) {
        /* generate all terms once */
        terms_len = 0;
        for (int i = 0; i < EDGE_COUNT; ++i) {
            int from = edges[i].from;
            int to   = edges[i].to;
            seq_t init = (seq_t)from * 10 + (seq_t)to;
            dfs(to, init, (1u << i), 1);
        }
        /* sort to provide sequence order */
        qsort(terms, terms_len, sizeof(seq_t), cmp_seq);
        terms_built = true;
    }

    if (n == 0 || (size_t)(n-1) >= terms_len)
        return 0;
    return terms[(size_t)(n-1)];
}
