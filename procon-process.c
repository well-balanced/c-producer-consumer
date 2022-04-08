#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdlib.h>

#define BUFFER_SIZE 20

int main()
{
    /*
     * 0으로 초기화된 버퍼 생성
     */
    int buffer[BUFFER_SIZE] = {0};
    /*
     * file discriptor 구조체 생성
     */
    int fds[2];
    int random;
    int i;
    pid_t pid;

    /*
     * 파이프 생성
     */
    pipe(fds);
    pid = fork();
    /*
     * pid가 0일 경우 자식 프로세스
     */
    bool is_child_process = pid == 0;
    if (is_child_process)
    {
        /*
         * 자식 프로세스
         */
        while (1)
        {
            /*
             * 파이프로부터 읽어와 버퍼에 저장
             */
            read(fds[0], &buffer, BUFFER_SIZE);
            for (i = 0; i < BUFFER_SIZE; i++)
            {
                /*
                 * 버퍼에 값이 truthy한 값일 경우에만 출력
                 */
                if (buffer[i])
                {
                    printf("child: %d 소비\n", buffer[i]);
                }
                else
                {
                    break;
                }
            }
            usleep(2000000);
        }
    }
    else
    {
        /*
         * 부모 프로세스
         */
        while (1)
        {
            /*
             * 랜덤 숫자를 생성한 후 파이프로 전달
             */
            random = rand();
            printf("parent: %d 생산\n", random);
            write(fds[1], &random, sizeof(random));
            usleep(1000000);
        }
    }
    return 0;
}