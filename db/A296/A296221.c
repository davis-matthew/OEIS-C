/*
 * A296221  Solution of the complementary equation a(n) = a(0)*b(n-1) +
 *          a(1)*b(n-2) + ... + a(n-1)*b(0) + 1, where a(0) = 1, a(1) = 3,
 *          b(0) = 2, and (a(n)) and (b(n)) are increasing complementary
 *          sequences.
 * https://oeis.org/A296221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A296221: a(0)=1, a(1)=3, b(0)=2,  a(n)=∑_{i=0}^{n-1}a(i)*b(n-1-i)+1,
   b is the complementary increasing sequence of the positive integers.
   The function a(idx_t n) returns a(n). */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"

/* static storage for the computed sequences */
static seq_t *a_vals = NULL;   /* a[0..a_len-1]  */
static size_t  a_len  = 0;     /* number of a values computed   */
static size_t  a_cap  = 0;     /* capacity of a_vals            */

static seq_t *b_vals = NULL;   /* b[0..b_len-1]  */
static size_t  b_len  = 0;     /* number of b values computed   */
static size_t  b_cap  = 0;     /* capacity of b_vals            */

static idx_t next_b_candidate = 3;   /* first integer that may belong to b */

/* simple binary search: returns 1 if val is present in arr[0..len-1] */
static int contains_a(const seq_t *arr, size_t len, seq_t val)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (arr[mid] < val) {
            lo = mid + 1;
        } else if (arr[mid] > val) {
            hi = mid;
        } else {
            return 1;            /* found */
        }
    }
    return 0;                    /* not found */
}

/* ensure that the internal arrays have space for at least 'need' entries */
static void grow_array(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        *arr = seq_realloc(*arr, *cap, new_cap);
        *cap = new_cap;
    }
}

/* extend the sequence so that a[n] is computed (0‑based). */
static void extend_to(idx_t n)
{
    /* initial values on first call */
    if (a_len == 0) {
        grow_array(&a_vals, &a_cap, 1);
        grow_array(&b_vals, &b_cap, 1);
        a_vals[0] = (seq_t)1;     /* a(0) */
        b_vals[0] = (seq_t)2;     /* b(0) */
        a_len = 1;
        b_len = 1;
    }

    while ((idx_t)a_len <= n) {
        /* --- compute a[a_len] --- */
        seq_t sum = 0;
        for (idx_t i = 0; i < (idx_t)a_len; ++i) {
            sum += a_vals[i] * b_vals[(idx_t)a_len - 1 - i];
        }
        seq_t new_a = sum + 1;

        /* store new a */
        grow_array(&a_vals, &a_cap, a_len + 1);
        a_vals[a_len] = new_a;
        ++a_len;

        /* --- compute the corresponding new b --- */
        idx_t cur = next_b_candidate;
        seq_t cur_val = (seq_t)cur;
        while (contains_a(a_vals, a_len, cur_val)) {
            ++cur;
            cur_val = (seq_t)cur;
        }
        seq_t new_b = cur_val;

        grow_array(&b_vals, &b_cap, b_len + 1);
        b_vals[b_len] = new_b;
        ++b_len;

        next_b_candidate = cur + 1;
    }
}

/* public function required by the problem statement */
seq_t a(idx_t n)
{
    extend_to(n);
    return a_vals[n];
}
