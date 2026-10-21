/*
 * A255545  Lucky / Unlucky array: Each row starts with n-th lucky number,
 *          followed by all unlucky numbers removed at stage n of the sieve.
 * https://oeis.org/A255545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) computed by simulating the lucky number sieve. Each row i
 *            starts with the i‑th lucky number; the next terms are the
 *            numbers removed at stage i when eliminating every step‑size
 *            element from the current list. The array is read in antidiagonal
 *            order to produce the sequence
 * Range:     u8 n <= 51, u16 n <= 3447, u32 n <= 5995, u64 n <= 5995,
 *            u128 n <= 5995, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#ifndef LIST_CAP
#define LIST_CAP 500000   /* capacity of the working list (should be large enough for n <= 10000) */
#endif

#ifndef MAX_REMOVED_PER_ROW
#define MAX_REMOVED_PER_ROW 200   /* enough for any j appearing in n <= 10000  */
#endif

/* Forward declarations of the required types: the build supplies seq_t,
   idx_t and the related utilities.  They are not defined here. */

/* Global state */
static seq_t *list_cur = NULL;   /* current survivors list */
static seq_t *list_next = NULL;  /* temporary storage for survivors of next stage */
static size_t cur_len = 0;       /* current length of survivors list */

/* Lucky numbers array */
static seq_t *lucky_numbers = NULL;
static size_t lucky_len = 0;
static size_t lucky_cap = 0;

/* Removed numbers per row */
static seq_t **removed_arr = NULL;     /* removed_arr[i] points to an array of removed numbers for row i+1 */
static size_t *removed_len = NULL;     /* number of removed numbers actually stored for each row */
static size_t removed_rows_cap = 0;     /* capacity of removed_arr, removed_len arrays */

/* Number of rows already generated */
static idx_t rows_generated = 0;

/* Helper to ensure the global structures are initialized */
static void ensure_initialized(void) {
    if (list_cur != NULL) return;  /* already initialized */

    /* Allocate working lists */
    list_cur = (seq_t *)malloc(LIST_CAP * sizeof(seq_t));
    list_next = (seq_t *)malloc(LIST_CAP * sizeof(seq_t));
    if (!list_cur || !list_next) {
        /* Allocation failed: abort silently */
        exit(1);
    }

    /* Initialize list_cur with 1..LIST_CAP */
    for (size_t i = 0; i < LIST_CAP; ++i) {
        list_cur[i] = (seq_t)(i + 1);
    }
    cur_len = LIST_CAP;

    /* Allocate lucky numbers array */
    lucky_cap = 64;
    lucky_numbers = (seq_t *)malloc(lucky_cap * sizeof(seq_t));
    if (!lucky_numbers) exit(1);

    /* Allocate removed numbers storage arrays */
    removed_rows_cap = 64;
    removed_arr = (seq_t **)calloc(removed_rows_cap, sizeof(seq_t *));
    removed_len = (size_t *)calloc(removed_rows_cap, sizeof(size_t));
    if (!removed_arr || !removed_len) exit(1);
}

/* Resize lucky numbers array if necessary */
static void ensure_lucky_capacity(size_t needed) {
    if (needed <= lucky_cap) return;
    size_t new_cap = lucky_cap;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(lucky_numbers, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    lucky_numbers = tmp;
    lucky_cap = new_cap;
}

/* Resize removed arrays storage if necessary */
static void ensure_removed_storage(size_t needed) {
    if (needed <= removed_rows_cap) return;
    size_t new_cap = removed_rows_cap;
    while (new_cap < needed) new_cap <<= 1;
    seq_t **new_arr = (seq_t **)realloc(removed_arr, new_cap * sizeof(seq_t *));
    size_t *new_len = (size_t *)realloc(removed_len, new_cap * sizeof(size_t));
    if (!new_arr || !new_len) exit(1);
    for (size_t i = removed_rows_cap; i < new_cap; ++i) {
        new_arr[i] = NULL;
        new_len[i] = 0;
    }
    removed_arr = new_arr;
    removed_len = new_len;
    removed_rows_cap = new_cap;
}

/* Generate one more stage of the sieve (row i+1).  Rows are 1‑based. */
static void generate_next_stage(void) {
    idx_t i = rows_generated + 1;   /* next row number (1‑based) */

    /* Ensure containers are large enough */
    ensure_lucky_capacity(i);
    ensure_removed_storage(i);

    /* Allocate array for removed numbers of this row */
    if (removed_arr[i - 1] == NULL) {
        removed_arr[i - 1] = (seq_t *)malloc(MAX_REMOVED_PER_ROW * sizeof(seq_t));
        if (!removed_arr[i - 1]) exit(1);
    }

    /* Determine step and lucky number for this stage */
    seq_t lucky_value;
    seq_t step;
    if (i == 1) {
        lucky_value = (seq_t)1;
        step = (seq_t)2;           /* first stage removes every 2nd number */
    } else {
        if ((size_t)i - 1 >= cur_len) {
            /* All remaining numbers are exhausted; nothing to generate further */
            lucky_value = (seq_t)0; step = (seq_t)1;
        } else {
            lucky_value = list_cur[(size_t)i - 1];   /* i‑th lucky number */
            step = lucky_value;                     /* step size equals the lucky number for i>=2 */
        }
    }
    lucky_numbers[i - 1] = lucky_value;

    /* Eliminate numbers from current list */
    size_t counter = 0;
    size_t stored_removed = 0;
    size_t next_len = 0;
    size_t step_usize = (size_t)step;   /* step fits into size_t for comparison */

    for (size_t idx = 0; idx < cur_len; ++idx) {
        seq_t val = list_cur[idx];
        counter++;
        if (counter == step_usize) {
            /* Remove this number */
            if (stored_removed < MAX_REMOVED_PER_ROW)
                removed_arr[i - 1][stored_removed] = val;
            stored_removed++;
            counter = 0;   /* reset counter after removal */
        } else {
            /* Keep this number for next stage */
            list_next[next_len++] = val;
        }
    }

    removed_len[i - 1] = stored_removed;

    /* Prepare for next generation */
    seq_t *tmp = list_cur;
    list_cur = list_next;
    list_next = tmp;
    cur_len = next_len;

    rows_generated = i;
}

/* Generate all rows up to (and including) the specified row number */
static void generate_up_to(idx_t target_row) {
    while (rows_generated < target_row) {
        generate_next_stage();
    }
}

/* Find the diagonal (sum i+j) that contains the n-th element */
static idx_t find_diagonal(idx_t n) {
    idx_t low = 1, high = 2;
    /* Expand high until enough */
    while ((unsigned __int128)high * (high - 1) / 2 < n) {
        high <<= 1;
    }
    /* Binary search for minimal d with d*(d-1)/2 >= n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid - 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* guard, though offset is 1 */

    ensure_initialized();

    /* Determine row i and column j from index n */
    idx_t diag = find_diagonal(n);                       /* d such that d*(d-1)/2 >= n  */
    idx_t prev_sum = (unsigned __int128)(diag - 1) * (diag - 2) / 2;  /* sum up to column 1 of previous diag */
    idx_t offset = n - prev_sum;                         /* position within diagonal (1‑based) */
    idx_t i = offset;                                     /* row number  */
    idx_t j = diag - offset;                              /* column number */

    /* Ensure we have generated enough rows */
    if (rows_generated < i) {
        generate_up_to(i);
    }

    /* Return the appropriate value */
    if (j == 1) {
        /* First column: the i‑th lucky number */
        return lucky_numbers[i - 1];
    } else {
        /* Subsequent columns: the (j‑2)-nd removed number of row i */
        size_t idx = j - 2;
        if (idx < removed_len[i - 1]) {
            return removed_arr[i - 1][idx];
        } else {
            /* In unlikely event of insufficient removed numbers, return 0 */
            return (seq_t)0;
        }
    }
}
