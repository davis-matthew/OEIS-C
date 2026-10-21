/*
 * A295364  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1)*b(n-2), where a(0) = 1, a(1) = 3, a[2] = 5, b(0) = 2, b(1)
 *          = 4, and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A295364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-1)*b(n-2) with a(0)=1, a(1)=3,
 *            a(2)=5, b(0)=2, b(1)=4, and a(), b() are increasing
 *            complementary sequences
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#define MAX_N 20000   /* sufficient for n up to about 10000 */

/* static storage for computed terms */
static seq_t a_cache[MAX_N];
static seq_t b_cache[MAX_N];

/* bookkeeping values – initialized on first call */
static idx_t max_a = (idx_t)-1;   /* highest index with a known term   */
static idx_t max_b = (idx_t)-1;   /* highest index with b known term   */
static seq_t next_b_candidate;    /* next integer to test for b        */
static idx_t a_ptr;               /* first a[pos] >= next_b_candidate */

/* Initialise with the base terms.  Called once on first use. */
static void init(void) {
    if (max_a != (idx_t)-1) return;   /* already done */

    /* base a-values */
    a_cache[0] = (seq_t)1;
    a_cache[1] = (seq_t)3;
    a_cache[2] = (seq_t)5;
    max_a = 2;

    /* base b-values */
    b_cache[0] = (seq_t)2;
    b_cache[1] = (seq_t)4;
    max_b = 1;

    next_b_candidate = (seq_t)5;   /* first candidate after b[1] */

    /* initialise a_ptr to the first a >= next_b_candidate */
    a_ptr = 0;
    while (a_ptr <= max_a && a_cache[a_ptr] < next_b_candidate)
        ++a_ptr;
}

/* Compute sequence terms up to index n (inclusive). */
static void compute_to(idx_t n) {
    init();

    if (n <= max_a) return;   /* already computed */

    for (idx_t i = max_a + 1; i <= n; ++i) {

        /* ensure that b[i-1] and b[i-2] are known */
        while (max_b < i - 1) {
            seq_t c = next_b_candidate;

            /* skip any a-value that equals c */
            while (1) {
                while (a_ptr <= max_a && a_cache[a_ptr] < c)
                    ++a_ptr;
                if (a_ptr <= max_a && a_cache[a_ptr] == c)
                    ++c;    /* c was used in a – try next integer */
                else
                    break;  /* c is free */
            }

            /* record the new b-value */
            b_cache[max_b + 1] = c;
            ++max_b;
            next_b_candidate = c + (seq_t)1;   /* next candidate starts after c */
        }

        /* compute a[i] */
        seq_t prod = b_cache[i - 1] * b_cache[i - 2];
        seq_t sum  = a_cache[i - 1] + a_cache[i - 2] + prod;
        a_cache[i] = sum;
        max_a = i;
    }
}

/* External function requested by the framework. */
seq_t a(idx_t n) {
    compute_to(n);
    return a_cache[n];
}
