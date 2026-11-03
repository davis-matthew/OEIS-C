/*
 * A395850  Divisors of the order of the Lyons group, |Ly| =
 *          51765179004000000.
 * https://oeis.org/A395850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full
 * Formula:   a(n) = A027750(A001228(19), n)
 * Range:     u8 none, u16 n <= 15, u32 n <= 3243, u64 n <= 16128,
 *            u128 n <= 16128, big any n
 * Validated: full, pass, 16128 terms through n = 16128 (2026-11-03)
 * Helpers:   factor_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/mem.h"

static int seq_cmp(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

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
        divs = mem_new(seq_t, cnt);
        if (divs == NULL)
            return 0;            /* allocation failure */

        /* Generate all divisors iteratively.
           Each stack element carries (prime index, current product). */
        typedef struct { size_t idx; seq_t val; } Item;
        Item *stack = mem_new(Item, cnt * 2);  /* buffer */
        if (stack == NULL) {
            mem_free(divs);
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
        mem_free(stack);

        /* Sort the divisors in increasing order */
        mem_qsort(divs, cnt, sizeof(seq_t), seq_cmp);
    }

    if (n < 1 || (idx_t)cnt < n)
        return 0;                     /* out of range */
    return divs[n - 1];              /* zero‑based index */
}
