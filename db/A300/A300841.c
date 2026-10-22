/*
 * A300841  Fermi-Dirac factorization prime shift towards larger terms: a(n) =
 *          A052330(2*A052331(n)).
 * https://oeis.org/A300841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = product of shifted Fermi‑Dirac factors, defined by a(n) =
 *            A052330(2*A052331(n)). The implementation builds the unique
 *            factorization sequence A050376 and multiplies the successors of
 *            its distinct factors
 * Range:     u8 n <= 1, u16 n <= 9806, u32 n <= 9806, u64 n <= 9806,
 *            u128 n <= 9806, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* ----------  STATIC STATE  ---------- */
static seq_t *A_array = NULL;   /* 1‑based array of A050376 values      */
static size_t A_len   = 0;       /* number of stored elements            */
static idx_t  A_limit = 0;       /* largest value for which A_array is valid */

/* ----------  GENERATE A050376  ---------- */
static void generate_A(idx_t limit)
{
    if (limit <= A_limit)
        return;                      /* already sufficient */

    /* Generate up to a bound that comfortably contains successors of
       every factor ≤ limit. The factor’s successor is at most
       2*limit + 1000 for the ranges we need. */
    idx_t new_limit = limit * 2 + 1000;
    if (new_limit <= A_limit)
        return;                      /* nothing to do */

    size_t max_size   = (size_t)(new_limit + 1);
    bool  *possible  = (bool*)calloc(max_size, sizeof(bool));
    if (!possible)
        return;
    size_t *prod_list = (size_t*)malloc(max_size * sizeof(size_t));
    if (!prod_list) { free(possible); return; }

    size_t prod_cnt  = 0;
    prod_list[prod_cnt++] = 1;          /* empty product */
    possible[1] = true;

    seq_t *newA = (seq_t*)malloc(max_size * sizeof(seq_t));
    if (!newA) { free(possible); free(prod_list); return; }
    newA[0] = 0;                        /* unused, keep 1‑based indexing */

    size_t count = 0;                    /* how many A values found */

    for (idx_t x = 2; x <= new_limit; ++x) {
        if (!possible[x]) {              /* x is the next A value */
            ++count;
            newA[count] = (seq_t)x;

            /* Update representable products using the new element
               only with products that existed before adding x. */
            size_t old_cnt = prod_cnt;
            for (size_t k = 0; k < old_cnt; ++k) {
                idx_t prod = prod_list[k] * x;
                if (prod <= new_limit && !possible[prod]) {
                    possible[prod] = true;
                    prod_list[prod_cnt++] = prod;
                }
            }
        }
    }

    free(possible);
    free(prod_list);
    free(A_array);

    A_array = newA;
    A_len   = count;
    A_limit = new_limit;
}

/* ----------  SEQUENCE FUNCTION  ---------- */
seq_t a(idx_t n)
{
    if (n == 1)
        return 1;                      /* a(1) = 1 */

    generate_A(n);

    seq_t result = 1;
    seq_t temp   = n;

    /* Greedy factorization using the largest available A[i] dividing temp */
    while (temp > 1) {
        for (idx_t i = A_len; i-- > 1; ) {   /* i runs from A_len‑1 down to 1 */
            seq_t ai = A_array[i];
            if (temp % ai == 0) {
                result *= A_array[i + 1];
                temp   /= ai;
                break;
            }
        }
    }

    return result;
}
