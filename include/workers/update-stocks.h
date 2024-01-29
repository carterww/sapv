#ifndef SAPV_UPDATE_STOCKS_H
#define SAPV_UPDATE_STOCKS_H

namespace sapv {
namespace worker {

void* fetch_thread_worker(void *portfolio_manager);

}  // namespace worker
}  // namespace sapv

#endif  //SAPV_UPDATE_STOCKS_H
