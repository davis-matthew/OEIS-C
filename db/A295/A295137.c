/*
 * A295137  Solution of the complementary equation a(n) = 3*a(n-1) + b(n-1) -
 *          n, where a(0) = 1, a(1) = 2, b(0) = 3, and (a(n)) and (b(n)) are
 *          increasing complementary sequences.
 * https://oeis.org/A295137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, b(0)=3. a(n)=3*a(n-1)+b(n-1)-n (n>=2) The
 *            sequences (a) and (b) are increasing complementary sequences:
 *            union of the two lists is the set of positive integers, each
 *            list strictly increasing. b(n) is the smallest unused integer
 *            greater than b(n-1)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation uses lazy memoisation.  For each n the function has accumulated
    all a(k), b(k) up to k=n in two dynamic arrays.  Finding the next unused
    integer uses binary search on the already known values of a() and b().  */

#include <stdlib.h>
#include <stdbool.h>

/* helper: binary search for seq_t in a sorted array */
static bool in_array(const seq_t *arr, idx_t count, seq_t val)
{
    idx_t lo = 0, hi = count;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (arr[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < count && arr[lo] == val);
}

/* helper: find the smallest integer > start that is not present in A or B */
static seq_t find_next_unused(seq_t start,
                              const seq_t *A, idx_t acnt,
                              const seq_t *B, idx_t bcnt)
{
    seq_t cand = start + (seq_t)1;
    while (true) {
        if (!in_array(A, acnt, cand) && !in_array(B, bcnt, cand))
            return cand;
        cand++;               /* candidate is guaranteed to be finite */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static memoisation tables */
    static seq_t *A = NULL, *B = NULL;
    static idx_t a_cap = 0, b_cap = 0;   /* storage capacities */
    static idx_t a_cnt = 0, b_cnt = 0;   /* number of elements stored */

    /* initialise on first call */
    if (A == NULL) {
        a_cap = 128;  b_cap = 128;
        A = (seq_t *)malloc(a_cap * sizeof(seq_t));
        B = (seq_t *)malloc(b_cap * sizeof(seq_t));

        /* base terms */
        A[0] = (seq_t)1;      /* a(0) */
        A[1] = (seq_t)2;      /* a(1) */
        B[0] = (seq_t)3;      /* b(0) */
        a_cnt = 2;            /* indices 0..1 present */
        b_cnt = 1;            /* index 0 present */
    }

    /* answer may already be known */
    if (n < a_cnt)
        return A[n];

    /* enlarge storage for a() if required */
    if (n + 1 > a_cap) {
        size_t newcap = a_cap ? a_cap * 2 : 128;
        if (newcap < n + 1) newcap = n + 1;
        A = (seq_t *)realloc(A, newcap * sizeof(seq_t));
        a_cap = newcap;
    }

    /* compute missing terms up to n */
    for (idx_t k = a_cnt; k <= n; ++k) {
        /* ensure that b(k-1) is available before computing a(k) */
        if (b_cnt <= k - 1) {
            seq_t newb = find_next_unused(B[b_cnt - 1], A, a_cnt, B, b_cnt);

            if (b_cnt >= b_cap) {
                size_t newcap = b_cap ? b_cap * 2 : 128;
                if (newcap < b_cnt + 1) newcap = b_cnt + 1;
                B = (seq_t *)realloc(B, newcap * sizeof(seq_t));
                b_cap = newcap;
            }
            B[b_cnt++] = newb;
        }

        /* compute a(k) */
        seq_t ak = (seq_t)3 * A[k - 1] + B[k - 1] - (seq_t)k;

        if (a_cnt >= a_cap) {
            size_t newcap = a_cap ? a_cap * 2 : 128;
            if (newcap < a_cnt + 1) newcap = a_cnt + 1;
            A = (seq_t *)realloc(A, newcap * sizeof(seq_t));
            a_cap = newcap;
        }
        A[a_cnt++] = ak;
    }

    return A[n];
}
