/*
 * A271419  If n is a ludic number, a(n)=0; if n is not a ludic number, a(n)
 *          is the ludic number that rejects n from the ludic number sieve.
 * https://oeis.org/A271419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is a Ludic number; otherwise the Ludic number that
 *            rejects n from the Ludic sieve
 * Range:     u8 n <= 2496, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static seq_t * _reject = NULL;
static idx_t   _max_n   = 0;

/* Build the Ludic sieve up to N and fill _reject array.
   _reject[x] = 0  if x is Ludic,
                 k  otherwise, where k is the Ludic number that removes x. */
static void _build_ludic(idx_t N)
{
    seq_t *new_table = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    if (!new_table) return;                     /* allocation failed */

    /* list of survivors; indices range 0 .. list_len-1 */
    idx_t *list = (idx_t *)malloc((size_t)(N + 1) * sizeof(idx_t));
    if (!list) { free(new_table); return; }

    for (idx_t i = 1; i <= N; ++i) list[i - 1] = i;
    idx_t list_len = N;

    /* iterate over the surviving numbers starting from the second one (value 2) */
    for (idx_t idx = 1; idx < list_len; ++idx) {
        idx_t step = list[idx];

        /* elimination for this step */
        idx_t new_len = 0;
        bool   after_step = false;
        idx_t  count      = 1;   /* count starts at 1 after the step itself */

        for (idx_t i = 0; i < list_len; ++i) {
            idx_t val = list[i];

            if (!after_step) {
                if (val == step) {
                    /* keep the step itself; its rejecter stays 0 */
                    after_step = true;
                    list[new_len++] = val;
                } else {
                    list[new_len++] = val;   /* numbers before the step are untouched */
                }
            } else {
                /* counting after the step */
                if (count % step == 0) {
                    /* removed by this step */
                    new_table[val] = step;
                } else {
                    list[new_len++] = val;
                }
                ++count;
            }
        }
        list_len = new_len;
    }

    free(list);

    free(_reject);
    _reject = new_table;
    _max_n  = N;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n<1 */

    if (n > _max_n) _build_ludic(n);
    return _reject[n];
}
