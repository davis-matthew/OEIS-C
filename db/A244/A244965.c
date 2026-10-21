/*
 * A244965  Triangle read by rows: T(n,k) = sum of the k-th divisor island of
 *          n.
 * https://oeis.org/A244965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the n‑th element of the triangle obtained by listing,
 *            row by row, the sums of the divisor islands of 1, 2, 3, …. The
 *            value for a given row is computed directly from the sorted list
 *            of that row’s divisors. No external table is used
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Standard headers  */
#include <stdlib.h>
#include <stdint.h>

/*  Comparator for qsort on uint64_t  */
static int cmp_uint64(const void *p, const void *q)
{
    const uint64_t *a = (const uint64_t *)p;
    const uint64_t *b = (const uint64_t *)q;
    return (*a > *b) - (*a < *b);
}

/*  Return the number of divisor islands of n.  */
static size_t island_count(idx_t n)
{
    uint64_t divs[256];          /* 256 > max number of divisors for n ≤ 10000 */
    size_t dcnt = 0;

    /*  Build the list of divisors (unsorted).  */
    for (uint64_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divs[dcnt++] = i;
            uint64_t j = n / i;
            if (j != i)
                divs[dcnt++] = j;
        }
    }

    /*  Sort the divisors.  */
    qsort(divs, dcnt, sizeof(uint64_t), cmp_uint64);

    /*  Count islands.  */
    size_t islands = 1;          /* at least one island (the first divisor) */
    for (size_t i = 1; i < dcnt; ++i) {
        if (divs[i] - divs[i - 1] > 1)
            ++islands;
    }
    return islands;
}

/*  Return the sum of the k‑th divisor island of n
    (k is 0‑based and 0 ≤ k < island_count(n)).  */
static seq_t island_sum_k(idx_t n, size_t k)
{
    uint64_t divs[256];
    size_t dcnt = 0;

    for (uint64_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divs[dcnt++] = i;
            uint64_t j = n / i;
            if (j != i)
                divs[dcnt++] = j;
        }
    }
    qsort(divs, dcnt, sizeof(uint64_t), cmp_uint64);

    size_t idx   = 0;                 /* current island index */
    seq_t  sum   = divs[0];           /* sum of the current island */

    for (size_t i = 1; i < dcnt; ++i) {
        if (divs[i] - divs[i - 1] > 1) {
            if (idx == k)
                return sum;
            ++idx;
            sum = divs[i];
        } else {
            sum += divs[i];
        }
    }
    /*  Last island  */
    if (idx == k)
        return sum;

    /*  Should never reach here for valid k.  */
    return (seq_t)0;
}

/*  Main sequence function.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                     /* undefined, safe default */

    idx_t target     = n - 1;                /* 0‑based position in the flattened triangle */
    idx_t cumulative = 0;                    /* number of elements already skipped */
    idx_t row        = 1;                    /* current row (1‑based) */

    for (;;) {
        size_t islands = island_count(row);

        if (cumulative + islands > target) {
            size_t k = target - cumulative;   /* 0‑based index within the current row */
            return island_sum_k(row, k);
        }

        cumulative += islands;
        ++row;
    }
}
