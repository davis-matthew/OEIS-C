/*
 * A332889  a(n) = number of strict partition numbers >1 that are proper
 *          divisors of the n-th strict partition number.
 * https://oeis.org/A332889
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of strict partition numbers >1 that are proper
 *            divisors of the nth strict partition number. For n >= 3.
 *            Implementation records p(k) – the number of partitions of k into
 *            distinct parts – in an array and counts the distinct p(k) values
 *            that divide p(n) except for 1 and p(n) itself. Th...
 * Range:     u8 n <= 28, u16 n <= 78, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array(size_t) */
#include "../utils/free_seq_array.h"    /* free_seq_array(seq_t*) */

/* Static tables holding the strict partition numbers and the sequence
   values.  They are grown to the maximum requested index. */
static seq_t *p_arr = NULL;      /* p(k) values */
static seq_t *a_arr = NULL;      /* a(k) values */
static size_t max_n = 0;         /* largest n for which tables exist */

/* Build tables up to the given limit (inclusive).  The function is static
   and defined at file scope. */
static void build_tables(size_t limit)
{
    seq_t *new_p = alloc_seq_array(limit + 1);
    seq_t *new_a = alloc_seq_array(limit + 1);

    /* Initialise all entries to zero.  (alloc_seq_array may already zero
       the memory, but we do it explicitly for clarity.) */
    for (size_t i = 0; i <= limit; ++i) {
        new_p[i] = 0;
        new_a[i] = 0;
    }
    /* p(0) = 1 (empty partition). */
    new_p[0] = 1;

    /* 0/1 knapsack DP: for each distinct part value 'part', update the
       counts for all sums from 'limit' down to 'part'. */
    for (size_t part = 1; part <= limit; ++part) {
        for (size_t sum = limit; sum >= part; --sum) {
            new_p[sum] = new_p[sum] + new_p[sum - part];
        }
    }

    /* Compute a(n) for all n >= 3 up to 'limit'. */
    /* a(0..2) remain 0. */
    for (size_t i = 3; i <= limit; ++i) {
        seq_t pn = new_p[i];
        seq_t count = 0;
        seq_t prev_m = 0;   /* to skip duplicate p(k) values */

        for (size_t k = 2; k < i; ++k) {   /* proper divisors only       */
            seq_t mk = new_p[k];
            if (mk <= 1) continue;         /* skip 1                         */
            if (mk == prev_m) continue;    /* duplicate p(k) value          */
            prev_m = mk;
            if (mk < pn && (pn % mk) == 0) {
                count = count + 1;
            }
        }
        new_a[i] = count;
    }

    /* Replace the old tables (if any) and free their memory. */
    if (p_arr) free_seq_array(p_arr);
    if (a_arr) free_seq_array(a_arr);
    p_arr = new_p;
    a_arr = new_a;
    max_n = limit;
}

/* External linkage function required by the exercise. */
seq_t a(idx_t n)
{
    if ((idx_t)n < 3) return 0;

    /* If the tables are not built up to this index yet, grow them. */
    if ((size_t)n > max_n) {
        build_tables((size_t)n);
    }
    return a_arr[n];
}
