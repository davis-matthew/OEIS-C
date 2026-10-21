/*
 * A256657  Numbers for which the minimal alternating Fibonacci representation
 *          has negative trace.
 * https://oeis.org/A256657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth positive integer whose minimal alternating Fibonacci
 *            representation has even length (negative trace)
 * Range:     u8 n <= 126, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are defined by the build system; no typedefs here. */

static int initialized = 0;
static seq_t *terms = NULL;
static idx_t terms_count = 0;

/* Return the n‑th term of sequence A256657.  n is 1‑based. */
seq_t a(idx_t n)
{
    if (!initialized) {
        /* ------------------------------------------------------------
           Parameters for the precomputation.  The limit is chosen to
           guarantee at least 10 000 terms; 250 000 gives a comfortable
           margin while staying small enough for the memory limits.
        */
        const idx_t LIMIT = 250000;     /* maximum integer considered   */
        const int   MAXFIB = 70;        /* to hold all Fibonacci numbers up to LIMIT */

        /* ---- Generate Fibonacci numbers up to LIMIT.  ----- */
        int fib[MAXFIB];
        fib[0] = 0;          /* unused, to keep array one‑based */
        fib[1] = 1;          /* F1 = 1  */
        fib[2] = 2;          /* F2 = 2  */
        int m = 2;           /* number of Fibonacci numbers stored (fib[1]..fib[m]) */
        while (1) {
            int next = fib[m - 1] + fib[m];
            if (next > (int)LIMIT) break;
            fib[++m] = next;
        }
        const int maxFib = m;

        /* ---- State representation for the BFS.  ---- */
        typedef struct {
            int  val;   /* current value reached (0 ≤ val ≤ LIMIT) */
            int  last;  /* last used Fibonacci index (1 ≤ last ≤ maxFib) */
            int  sign;  /* 0 → next term is +, 1 → next term is – */
            int  step;  /* number of terms used so far              */
        } State;

        /* ---- Allocate visited array and temporary data.  ---- */
        size_t total_states = ((size_t)(LIMIT + 1) * (maxFib + 1) * 2);
        uint8_t *visited = (uint8_t *)calloc(total_states, 1);
        int *min_steps = (int *)calloc(LIMIT + 1, sizeof(int));
        for (int i = 0; i <= LIMIT; ++i) min_steps[i] = -1;

        /* ---- Queue for BFS.  ---- */
        State *queue = (State *)malloc(total_states * sizeof(State));
        size_t head = 0, tail = 0;

        /* ---- Helper to compute index into the visited array. ---- */
        #define IDX(val,last,sign) ( ((size_t)(val) * (maxFib + 1) + (size_t)(last)) << 1 | (unsigned)(sign) )

        /* ---- Initial state: value 0, any last index, next sign +.  ---- */
        int start_last = maxFib;            /* allows any Fibonacci index       */
        int start_sign = 0;                 /* 0 → next term is +                */
        visited[IDX(0, start_last, start_sign)] = 1;
        queue[tail++] = (State){0, start_last, start_sign, 0};

        /* ---- Breadth‑first search.  ---- */
        while (head < tail) {
            State cur = queue[head++];
            int val   = cur.val;
            int last  = cur.last;
            int sign  = cur.sign;          /* 0 → + , 1 → – */
            int step  = cur.step;

            if (val > 0 && min_steps[val] == -1)
                min_steps[val] = step;

            if (last == 0) continue;       /* no smaller index available */

            for (int i = 1; i <= last; ++i) {
                int delta = (sign == 0) ? fib[i] : -fib[i];
                int new_val = val + delta;
                if (new_val < 0 || new_val > (int)LIMIT) continue;

                int new_last = i;
                int new_sign = 1 - sign;   /* alternate the sign */
                size_t vdx = IDX(new_val, new_last, new_sign);
                if (!visited[vdx]) {
                    visited[vdx] = 1;
                    queue[tail++] = (State){new_val, new_last, new_sign, step + 1};
                }
            }
        }

        /* ---- Build the list of terms with even minimal length ----- */
        terms = (seq_t *)malloc(LIMIT * sizeof(seq_t));
        terms_count = 0;
        for (int k = 1; k <= (int)LIMIT; ++k) {
            if (min_steps[k] != -1 && (min_steps[k] & 1) == 0) {
                terms[terms_count++] = (seq_t)k;
            }
        }

        /* ---- Clean up temporary arrays. ---- */
        free(visited);
        free(min_steps);
        free(queue);

        initialized = 1;
    }

    if (n == 0 || n > terms_count) return 0;
    return terms[n - 1];
}
