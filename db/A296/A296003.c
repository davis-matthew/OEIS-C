/*
 * A296003  Solution of the complementary equation a(n) = a(0)*b(n-1) +
 *          a(1)*b(n-2) + ... + a(n-1)*b(0), where a(0) = 2, a(1) = 4, b(0) =
 *          1, and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0}^{n-1} a(k)*b(n-1-k)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0)=2, a(1)=4, b(0)=1, b(1)=3, ...   */

#include <stdlib.h>

/* Binary search: does val appear in the sorted array arr[0..len-1]? */
static int contains(seq_t val, const seq_t *arr, size_t len)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid] < val)
            lo = mid + 1;
        else if (arr[mid] > val)
            hi = mid;
        else
            return 1;
    }
    return 0;
}

/* Ensure that *p has room for at least need elements */
static void grow_seq(seq_t **p, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        while (new_cap < need)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*p, new_cap * sizeof(seq_t));
        *p = tmp;
        *cap = new_cap;
    }
}

/* sequence A296003: complementary product recurrence */
seq_t a(idx_t n)
{
    static seq_t *a_arr = NULL;      /* a sequence */
    static size_t a_cap = 0, a_len = 0;
    static seq_t *b_arr = NULL;      /* b complementary sequence */
    static size_t b_cap = 0, b_len = 0;
    static idx_t cur_b = 1;          /* smallest integer not yet examined */

    /* Initialise a(0) and a(1) once */
    if (a_len == 0) {
        grow_seq(&a_arr, &a_cap, 2);
        a_arr[0] = (seq_t)2;
        a_arr[1] = (seq_t)4;
        a_len = 2;
    }

    /* Extend the sequences up to the requested index n */
    while (a_len <= (size_t)n) {
        idx_t m = (idx_t)a_len;       /* index to compute next */

        /* Build b[0..m-1] as needed */
        while (b_len <= (size_t)(m - 1)) {
            while (1) {
                if (!contains((seq_t)cur_b, a_arr, a_len)) {
                    grow_seq(&b_arr, &b_cap, b_len + 1);
                    b_arr[b_len++] = (seq_t)cur_b;
                    ++cur_b;        /* advance past the value just inserted */
                    break;
                }
                ++cur_b;            /* skip values already in a */
            }
        }

        /* Compute a(m) = Σ_{k=0}^{m-1} a(k) * b(m-1-k) */
        seq_t sum = 0;
        for (size_t k = 0; k < (size_t)m; ++k)
            sum += a_arr[k] * b_arr[(size_t)(m - 1 - k)];

        grow_seq(&a_arr, &a_cap, a_len + 1);
        a_arr[a_len++] = sum;
    }

    return a_arr[(size_t)n];
}
