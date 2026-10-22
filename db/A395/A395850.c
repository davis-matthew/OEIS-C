/*
 * A395850  Divisors of the order of the Lyons group, |Ly| =
 *          51765179004000000.
 * https://oeis.org/A395850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = A027750(A001228(19), n)
 * Range:     u8 none, u16 n <= 15, u32 n <= 3600, u64 n <= 9746,
 *            u128 n <= 9746, big any n
 * Helpers:   factor_u64, seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_cmp.h"

seq_t a(idx_t n)
{
    /* Static cache of all divisors of |Ly| */
    static seq_t *divs = NULL;
    static size_t cnt  = 0;

    /* Compute once */
    if (divs == NULL) {
        /* Order of the Lyons group */
        const uint64_t LY_ORDER = 51765179004000000ULL;

        /* Prime factorisation */
        uint64_t primes[64], exps[64];
        size_t num = 0;
        factor_u64(LY_ORDER, primes, exps, &num);

        /* Number of divisors = product (exp_i + 1) */
        cnt = 1;
        for (size_t i = 0; i < num; ++i)
            cnt *= (size_t)(exps[i] + 1);

        /* Allocate storage for members of the divisor set */
        divs = (seq_t *)malloc(cnt * sizeof(seq_t));
        if (divs == NULL)
            return 0;            /* allocation failure */

        /* Generate all divisors iteratively.
           Each stack element carries (prime index, current product). */
        typedef struct { size_t idx; seq_t val; } Item;
        Item *stack = (Item *)malloc(cnt * 2 * sizeof(Item));  /* buffer */
        if (stack == NULL) {
            free(divs);
            divs = NULL;
            return 0;
        }

        size_t top = 0;
        stack[top++] = (Item){0, (seq_t)1};

        size_t pos = 0;
        while (top > 0) {
            Item it = stack[--top];
            size_t i  = it.idx;
            seq_t  v  = it.val;

            if (i == num) {               /* all primes processed */
                divs[pos++] = v;
                continue;
            }

            /* Exponent 0 for this prime */
            stack[top++] = (Item){i + 1, v};

            /* Exponents 1 .. exps[i] */
            seq_t w = v;
            for (uint8_t e = 1; e <= exps[i]; ++e) {
                w *= (seq_t)primes[i];
                stack[top++] = (Item){i + 1, w};
            }
        }
        free(stack);

        /* Sort the divisors in increasing order */
        qsort(divs, cnt, sizeof(seq_t), seq_cmp);
    }

    if (n < 1 || (idx_t)cnt < n)
        return 0;                     /* out of range */
    return divs[n - 1];              /* zero‑based index */
}
